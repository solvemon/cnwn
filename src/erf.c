#include "cnwn/erf.h"


#define FREE_REGEXP_THINGY if (compiled_regexps != NULL) { for (int reindex = 0; reindex < compiled_regexps_count; reindex++) regfree(compiled_regexps + reindex); free(compiled_regexps); }


static int64_t cnwn_erf_default_handler(const cnwn_ERFHandlers * handlers, cnwn_ERFHandlerMode mode, const char * path, int fd, const char * output_path, int output_fd, const cnwn_ERFHeader * header, const cnwn_ERFEntry * entry)
{
    if (mode == CNWN_ERF_HANDLER_MODE_EXTRACT) {
        if (entry->resource_size > 0) 
            return cnwn_copy_bytes(fd, output_fd, entry->resource_size);
        return 0;
    } else if (mode == CNWN_ERF_HANDLER_MODE_ARCHIVE) {
        return 0;
    }
    cnwn_set_error("invalid mode (%d)", mode);
    return -1;
}

const cnwn_ERFHandler CNWN_ERF_DEFAULT_HANDLER = cnwn_erf_default_handler;


int cnwn_erf_read_contents(int fd, const char * regexps[], bool extended, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries)
{
    regex_t * compiled_regexps = NULL;
    int compiled_regexps_count = 0;
    if (regexps != NULL) {
        while (regexps[compiled_regexps_count] != NULL)
            compiled_regexps_count++;
        compiled_regexps = malloc(sizeof(regex_t) * compiled_regexps_count);
        int flags = REG_ICASE;
        if (extended)
            flags |= REG_EXTENDED;
        for (int i = 0; i < compiled_regexps_count; i++) {
            int rexret = regcomp(compiled_regexps + i, regexps[i], flags);
            if (rexret != 0) {
                char tmps[1024];
                regerror(rexret, compiled_regexps + i, tmps, sizeof(tmps));
                cnwn_set_error("regexp #%d is invalid, %s", i, tmps);
                FREE_REGEXP_THINGY;
                return -1;
            }
        }
    }
    cnwn_ERFHeader tmpheader = {0};
    cnwn_ERFHeader * header = (ret_header != NULL ? ret_header : &tmpheader);
    uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
    char tmps[128];
    int tmpsoffset = 0;
    int64_t ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header type, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    for (int64_t i = 0; i < ret && i < 4; i++) {
        header->type_str[i] = data[i];
        if (data[i] > 32 && data[i] < 128)
            tmps[tmpsoffset++] = ((char)data[i] >= 'A' && (char)data[i] <= 'Z' ? data[i] + 32 : data[i]);
    }
    tmps[tmpsoffset] = 0;
    header->type = cnwn_resource_type_from_extension(tmps);
    if (header->type <= CNWN_RESOURCE_TYPE_NONE || header->type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid resource type '%s'", tmps);
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header version, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    for (int64_t i = 0; i < ret && i < 4; i++)
        header->version_str[i] = data[i];
    if ((header->version_str[0] == 'V' || header->version_str[0] == 'v')
        && header->version_str[1] >= '0' && header->version_str[1] <= '9'
        && header->version_str[2] == '.'
        && header->version_str[3] >= '0' && header->version_str[3] <= '9') {
        header->version.major = (int)header->version_str[1] - (int)'0';
        header->version.minor = (int)header->version_str[3] - (int)'0';
    } else {
        cnwn_set_error("invalid version '%s'", header->version_str);
        FREE_REGEXP_THINGY;
        return -1;
    }
    if (header->version.major != 1 || header->version.minor > 1) {
        cnwn_set_error("unsupported version %d.%d", header->version.major, header->version.minor);
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings count, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings size, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->num_entries);
    if (ret < 0) {
        cnwn_set_error("error reading header entries count, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings offset, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->keys_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header keys offset, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->resources_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header resources offset, %s", cnwn_get_error());
        FREE_REGEXP_THINGY;
        return -1;
    }
    if (header->num_localized_strings > 0 && header->localized_strings_offset < 160) {
        cnwn_set_error("invalid localized strings offset %u", header->localized_strings_offset);
        FREE_REGEXP_THINGY;
        return -1;
    }
    if (header->num_entries > 0 && header->keys_offset < 160) {
        cnwn_set_error("invalid keys offset %u", header->keys_offset);
        FREE_REGEXP_THINGY;
        return -1;
    }
    if (header->num_entries > 0 && header->resources_offset < 160) {
        cnwn_set_error("invalid resources offset %u", header->resources_offset);
        FREE_REGEXP_THINGY;
        return -1;
    }
    int num_entries = header->num_entries;
    max_entries = CNWN_MAX(0, max_entries);
    if (max_entries > 0)
        num_entries = CNWN_MIN(num_entries, max_entries);
    else if (ret_entries != NULL) {
        FREE_REGEXP_THINGY;
        return 0;
    }
    cnwn_ERFEntry * entries = malloc(sizeof(cnwn_ERFEntry) * num_entries);
    memset(entries, 0, sizeof(cnwn_ERFEntry) * num_entries);
    ret = cnwn_seek(fd, header->keys_offset);
    if (ret < 0) {
        cnwn_set_error("error seeking keys offset (%s)", cnwn_get_error());
        free(entries);
        FREE_REGEXP_THINGY;
        return -1;
    }
    int key_size = (header->version.minor > 0 ? 32 : 16);
    for (int i = 0; i < num_entries; i++) {
        entries[i].key_offset = header->keys_offset + i * (key_size + 8);
        ret = cnwn_read_bytes(fd, key_size, data);
        if (ret < 0) {
            cnwn_set_error("error reading key for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        if (ret < key_size) {
            cnwn_set_error("not enough data to read key for entry #%d (%"PRId64")", i, ret);
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        for (int j = 0; j < ret; j++)
            if (data[j] > 32 && data[j] < 128)
                entries[i].key[j] = data[j];
            else {
                entries[i].key[j] = 0;
                break;
            }
        ret = cnwn_read_uint32(fd, &entries[i].resource_id);
        if (ret < 0) {
            cnwn_set_error("error reading resource ID for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        ret = cnwn_read_uint16(fd, &entries[i].erf_type);
        if (ret < 0) {
            cnwn_set_error("error reading ERF type for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        entries[i].type = cnwn_resource_type_from_erf_type(entries[i].erf_type);
        if (entries[i].type <= CNWN_RESOURCE_TYPE_NONE || entries[i].type >= CNWN_MAX_RESOURCE_TYPE) {
            cnwn_set_error("invalid ERF resource type for entry #%d (%u)", i, entries[i].erf_type);
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        ret = cnwn_read_uint16(fd, &entries[i].unused);
        if (ret < 0) {
            cnwn_set_error("error reading unused for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
    }
    ret = cnwn_seek(fd, header->resources_offset);
    if (ret < 0) {
        cnwn_set_error("error seeking resources offset (%s)", cnwn_get_error());
        free(entries);
        FREE_REGEXP_THINGY;
        return -1;
    }
    for (int i = 0; i < num_entries; i++) {
        ret = cnwn_read_uint32(fd, &entries[i].resource_offset);
        if (ret < 0) {
            cnwn_set_error("error reading resource offset for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        if (entries[i].resource_offset < header->resources_offset) {
            cnwn_set_error("invalid resource offset for entry #%d (%u)", i, entries[i].resource_offset);
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
        ret = cnwn_read_uint32(fd, &entries[i].resource_size);
        if (ret < 0) {
            cnwn_set_error("error reading resource size for entry #%d (%s)", i, cnwn_get_error());
            free(entries);
            FREE_REGEXP_THINGY;
            return -1;
        }
    }
    int retnum_entries = 0;
    for (int i = 0; i < num_entries; i++) {
        if (compiled_regexps != NULL) {
            char tmppath[CNWN_PATH_MAX_SIZE];
            cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(tmppath), tmppath);
            for (int j = 0; j < compiled_regexps_count; j++) {
                if (regexec(compiled_regexps + j, tmppath, 0, NULL, 0) == 0) {
                    if (ret_entries != NULL)
                        ret_entries[retnum_entries] = entries[i];
                    retnum_entries++;
                break;
                }
            }
        } else {
            if (ret_entries != NULL)
                ret_entries[retnum_entries] = entries[i];
            retnum_entries++;
        }
    }
    free(entries);
    FREE_REGEXP_THINGY;
    return retnum_entries;
}

int cnwn_erf_read_contents_path(const char * path, const char * regexps[], bool extended, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries)
{
    int fd = cnwn_open_read(path);
    if (fd < 0) {
        cnwn_set_error("error opening \"%s\" (%s)", path, strerror(errno));
        return -1;
    }
    int ret = cnwn_erf_read_contents(fd, regexps, extended, ret_header, max_entries, ret_entries);
    cnwn_close(fd);
    return ret;
}

int cnwn_erf_extract(const char * path, const char * regexps[], bool extended, const char * output_dir, const cnwn_ERFHandlers * handlers, FILE * verbose_output, int * ret_files, int64_t * ret_bytes)
{
    int fd = cnwn_open_read(path);
    if (fd < 0) {
        cnwn_set_error("could not open file (%s): %s", strerror(errno), path);
        return -1;
    }
    cnwn_ERFHeader header = {0};
    int num_entries = cnwn_erf_read_contents(fd, regexps, extended, &header, 0, NULL);
    if (num_entries <= 0) {
        cnwn_close(fd);
        return num_entries;
    }
    int seeky = cnwn_seek(fd, 0);
    if (seeky < 0) {
        cnwn_close(fd);
        return -1;
    }
    cnwn_ERFEntry * entries = malloc(sizeof(cnwn_ERFEntry) * num_entries);
    int num_entries2 = cnwn_erf_read_contents(fd, regexps, extended, &header, num_entries, entries);
    if (num_entries2 < 0) {
        cnwn_close(fd);
        free(entries);
        return -1;
    }
    if (num_entries2 != num_entries) {
        cnwn_set_error("probably a bug, the first cnwn_erf_read_contents() returned %d and the second %d", num_entries, num_entries2);
        cnwn_close(fd);
        free(entries);
        return -1;
    }
    if (output_dir != NULL && output_dir[0] != 0) {
        int mkd = cnwn_mkdirs(output_dir);
        if (mkd < 0) {
            cnwn_set_error("could not create output directory, %s", cnwn_get_error());
            cnwn_close(fd);
            free(entries);
            return -1;
        } else if (mkd > 0)
            printf("Created output path: %s\n", output_dir);
    }
    int64_t num_bytes = 0;
    int num_files = 0;
    int ret = 0;
    for (int i = 0; i < num_entries2; i++) {
        if (CNWN_RESOUCE_TYPE_VALID(entries[i].type)) {
            cnwn_ERFHandler handler = CNWN_ERF_DEFAULT_HANDLER;
            if (handlers != NULL) {
                if (handlers->type_handler[entries[i].type] != NULL)
                    handler = handlers->type_handler[entries[i].type];
                else if (handlers->handler != NULL)
                    handler = handlers->handler;
            }
            char filename[CNWN_PATH_MAX_SIZE];
            cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(filename), filename);
            if (filename[0] != 0) {
                char output_path[CNWN_PATH_MAX_SIZE];
                cnwn_path_append(output_dir, filename, sizeof(output_path), output_path);
                int seek_ret = cnwn_seek(fd, entries[i].resource_offset);
                if (seek_ret < 0) {
                    cnwn_set_error("entry #%d seek %s: %s", i, cnwn_get_error(), output_path);
                    ret = -1;
                    break;
                }
                int output_fd = cnwn_open_write(output_path);
                if (output_fd < 0) {
                    cnwn_set_error("entry #%d output %s: %s", i, cnwn_get_error(), output_path);
                    ret = -1;
                    break;
                }
                int64_t handler_bytes = handler(handlers, CNWN_ERF_HANDLER_MODE_EXTRACT, path, fd, output_path, output_fd, &header, entries + i);
                cnwn_close(output_fd);
                if (handler_bytes < 0) {
                    cnwn_set_error("entry #%d handler %s: %s", i, cnwn_get_error(), output_path);
                    ret = -1;
                    break;
                }
                if (verbose_output != NULL)
                    fprintf(verbose_output, "Extracted %"PRId64" bytes: %s\n", handler_bytes, output_path);
                num_bytes += handler_bytes;
                num_files++;
                ret++;
            }
        }
    }
    cnwn_close(fd);
    free(entries);
    if (ret_bytes != NULL)
        *ret_bytes = num_bytes;
    if (ret_files != NULL)
        *ret_files = num_files;
    return ret;
}
