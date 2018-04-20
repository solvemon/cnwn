#include "cnwn/erf.h"

int cnwn_erf_header_to_string(const cnwn_ERFHeader * header, bool detailed, int max_size, char * ret_s)
{
    char tmps[1024];
    if (header != NULL) {
        if (detailed)
            snprintf(tmps, sizeof(tmps), "%s%s%s%s%s %d.%d %s%s%s %"PRId64" bytes",
                     CNWN_RESOURCE_TYPE_EXTENSION(header->type),
                     cnwn_string_isempty(CNWN_RESOURCE_TYPE_EXTENSION(header->type)) ? "" : " ",
                     cnwn_string_isempty(header->type_str) ? "" : "(",
                     header->type_str,
                     cnwn_string_isempty(header->type_str) ? "" : ")",
                     header->version.major,
                     header->version.minor,
                     cnwn_string_isempty(header->version_str) ? "" : "(",
                     header->version_str,
                     cnwn_string_isempty(header->version_str) ? "" : ")",
                     header->filesize
                     );
        else
            snprintf(tmps, sizeof(tmps), "%s%s%d.%d",
                     CNWN_RESOURCE_TYPE_EXTENSION(header->type),
                     cnwn_string_isempty(CNWN_RESOURCE_TYPE_EXTENSION(header->type)) ? "" : " ",
                     header->version.major,
                     header->version.minor
                     );
    } else
        tmps[0] = 0;
    return cnwn_copy_string(ret_s, max_size, tmps, -1);
}

int cnwn_erf_entry_to_string(const cnwn_ERFEntry * entry, bool detailed, int max_size, char * ret_s)
{
    char tmps[1024];
    if (entry != NULL) {
        char filename[CNWN_PATH_MAX_SIZE];
        cnwn_resource_type_and_key_to_filename(entry->type, entry->key, sizeof(filename), filename);
        if (detailed)
            snprintf(tmps, sizeof(tmps), "%s%s%u bytes at 0x%x (erftype=%u, key %d at 0x%x, resource ID %u)",
                     filename,
                     cnwn_string_isempty(filename) ? "": ", ",
                     entry->resource_size,
                     entry->resource_offset,
                     entry->itype,
                     entry->index,
                     entry->key_offset,
                     entry->resource_id
                     );
        else
            snprintf(tmps, sizeof(tmps), "%s, %u bytes", filename, entry->resource_size);
    } else
        tmps[0] = 0;
    return cnwn_copy_string(ret_s, max_size, tmps, -1);
}


int cnwn_erf_read_header(cnwn_File * f, cnwn_ERFHeader * ret_header, cnwn_ERFEntry ** ret_entries, int64_t * ret_bytes)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
    int64_t did_bytes = 0;
    cnwn_ERFHeader header = {0};
    uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
    int64_t ret = cnwn_file_read(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header type, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    for (int64_t i = 0; i < ret && i < 4; i++)
        header.type_str[i] = data[i];
    header.type = cnwn_resource_type_from_extension(header.type_str);
    if (header.type <= CNWN_RESOURCE_TYPE_NONE || header.type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid ERF type '%s'", header.type_str);
        return -1;
    }
    ret = cnwn_file_read(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header version, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    for (int64_t i = 0; i < ret && i < 4; i++)
        header.version_str[i] = data[i];
    if ((header.version_str[0] == 'V' || header.version_str[0] == 'v')
        && header.version_str[1] >= '0' && header.version_str[1] <= '9'
        && header.version_str[2] == '.'
        && header.version_str[3] >= '0' && header.version_str[3] <= '9') {
        header.version.major = (int)header.version_str[1] - (int)'0';
        header.version.minor = (int)header.version_str[3] - (int)'0';
    } else {
        cnwn_set_error("invalid version '%s'", header.version_str);
        return -1;
    }
    if (header.version.major != 1 || header.version.minor < 0 || header.version.minor > 1) {
        cnwn_set_error("unsupported version %d.%d", header.version.major, header.version.minor);
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings count, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_read_uint32(f, &header.localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings size, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_read_uint32(f, &header.num_entries);
    if (ret < 0) {
        cnwn_set_error("error reading header entries count, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_read_uint32(f, &header.localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_read_uint32(f, &header.keys_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header keys offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_read_uint32(f, &header.resources_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header resources offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    if (header.num_localized_strings > 0 && header.localized_strings_offset < 160) {
        cnwn_set_error("invalid localized strings offset %u", header.localized_strings_offset);
        return -1;
    }
    if (header.num_entries > 0 && header.keys_offset < 160) {
        cnwn_set_error("invalid keys offset %u", header.keys_offset);
        return -1;
    }
    if (header.num_entries > 0 && header.resources_offset < 160) {
        cnwn_set_error("invalid resources offset %u", header.resources_offset);
        return -1;
    }
    ret = cnwn_file_read(f, 128, header.rest);
    if (ret < 0) {
        cnwn_set_error("could not read the remainder of the header, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_seek_end(f);
    if (ret < 0) {
        cnwn_set_error("could not seek end, %s", cnwn_get_error());
        return -1;
    }
    header.filesize = cnwn_file_seek_cur(f);
    if (header.filesize < 0) {
        cnwn_set_error("could not get current seek, %s", cnwn_get_error());
        return -1;
    }
    if (ret_entries == NULL) {
        if (ret_header != NULL)
            *ret_header = header;
        return header.num_entries;
    }
    cnwn_ERFEntry * entries = NULL;
    if (header.num_entries > 0) {
        entries = malloc(sizeof(cnwn_ERFEntry) * header.num_entries);
        memset(entries, 0, sizeof(cnwn_ERFEntry) * header.num_entries);
        ret = cnwn_file_seek(f, header.keys_offset);
        if (ret < 0) {
            cnwn_set_error("error seeking keys offset (%s)", cnwn_get_error());
            free(entries);
            return -1;
        }
        int key_size = (header.version.minor > 0 ? 32 : 16);
        for (int i = 0; i < header.num_entries; i++) {
            entries[i].index = i;
            entries[i].key_offset = header.keys_offset + i * (key_size + 8);
            ret = cnwn_file_read(f, key_size, data);
            if (ret < 0) {
                cnwn_set_error("error reading key for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            did_bytes += ret;
            if (ret < key_size) {
                cnwn_set_error("not enough data to read key for entry #%d (%"PRId64")", i, ret);
                free(entries);
                return -1;
            }
            for (int j = 0; j < ret; j++)
                if (data[j] > 32 && data[j] < 128)
                    entries[i].key[j] = data[j];
                else {
                    entries[i].key[j] = 0;
                    break;
                }
            ret = cnwn_file_read_uint32(f, &entries[i].resource_id);
            if (ret < 0) {
                cnwn_set_error("error reading resource ID for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            did_bytes += ret;
            ret = cnwn_file_read_uint16(f, &entries[i].itype);
            if (ret < 0) {
                cnwn_set_error("error reading ERF type for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            did_bytes += ret;
            entries[i].type = cnwn_resource_type_from_itype(entries[i].itype);
            if (entries[i].type <= CNWN_RESOURCE_TYPE_NONE || entries[i].type >= CNWN_MAX_RESOURCE_TYPE) {
                cnwn_set_error("invalid ERF resource type for entry #%d (%u)", i, entries[i].itype);
                free(entries);
                return -1;
            }
            ret = cnwn_file_read_uint16(f, &entries[i].unused);
            if (ret < 0) {
                cnwn_set_error("error reading unused for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            did_bytes += ret;
        }
        ret = cnwn_file_seek(f, header.resources_offset);
        if (ret < 0) {
            cnwn_set_error("error seeking resources offset (%s)", cnwn_get_error());
            free(entries);
            return -1;
        }
        for (int i = 0; i < header.num_entries; i++) {
            ret = cnwn_file_read_uint32(f, &entries[i].resource_offset);
            if (ret < 0) {
                cnwn_set_error("error reading resource offset for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            did_bytes += ret;
            if (entries[i].resource_offset < header.resources_offset) {
                cnwn_set_error("invalid resource offset for entry #%d (%u)", i, entries[i].resource_offset);
                free(entries);
                return -1;
            }
            ret = cnwn_file_read_uint32(f, &entries[i].resource_size);
            if (ret < 0) {
                cnwn_set_error("error reading resource size for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
        }
    }
    if (ret_header != NULL)
        *ret_header = header;
    if (ret_entries != NULL)
        *ret_entries = entries;
    else
        free(entries);
    if (ret_bytes != NULL)
        *ret_bytes = did_bytes;
    return header.num_entries;
}

int cnwn_erf_write_header(cnwn_File * f, const cnwn_ERFHeader * header, int num_entries, const cnwn_ERFEntry * entries, int64_t * ret_bytes)
{
    if (header->version.major != 1 || header->version.minor < 0 || header->version.minor > 1) {
        cnwn_set_error("unsupported version %d.%d", header->version.major, header->version.minor);
        return -1;
    }
    int64_t did_bytes = 0;
    uint8_t data[5] = {0};
    const char * extension = CNWN_RESOURCE_TYPE_EXTENSION(header->type);
    for (int i = 0; i < 3; i++)
        if (extension[i] >= 'a' && extension[i] <= 'z')
            data[i] = extension[i] - 32;
        else if (extension[i] < 32)
            data[i] = 32;
        else
            data[i] = extension[i];
    int64_t ret = cnwn_file_write(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("could not write ERF type, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    char tmps[5];
    snprintf(tmps, sizeof(tmps), "V%d.%d", header->version.major, header->version.minor);
    for (int i = 0; i < 4; i++)
        data[i] = tmps[i];
    ret = cnwn_file_write(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("could not write ERF version, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("could not write num localized strings, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("could not write localized strings size, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->num_entries);
    if (ret < 0) {
        cnwn_set_error("could not write num entries, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("could not write localized strings offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->keys_offset);
    if (ret < 0) {
        cnwn_set_error("could not write keys offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write_uint32(f, header->resources_offset);
    if (ret < 0) {
        cnwn_set_error("could not write resources offset, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    ret = cnwn_file_write(f, 128, header->rest);
    if (ret < 0) {
        cnwn_set_error("could not write rest, %s", cnwn_get_error());
        return -1;
    }
    did_bytes += ret;
    if (header->num_entries > 0) {
        ret = cnwn_file_seek(f, header->keys_offset);
        if (ret < 0) {
            cnwn_set_error("could not seek keys, %s", cnwn_get_error());
            return -1;
        }
        int key_size = (header->version.minor > 0 ? 32 : 16);
        for (int i = 0; i < header->num_entries; i++) {
            uint8_t tmpdata[32] = {0};
            for (int j = 0; j < 32 && entries[i].key[j] != 0; j++)
                tmpdata[j] = entries[i].key[j];
            ret = cnwn_file_write(f, key_size, tmpdata);
            if (ret < 0) {
                cnwn_set_error("could not write key for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
            ret = cnwn_file_write_uint32(f, entries[i].resource_id);
            if (ret < 0) {
                cnwn_set_error("could not write resource ID for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
            ret = cnwn_file_write_uint16(f, CNWN_RESOURCE_TYPE_ITYPE(entries[i].type));
            if (ret < 0) {
                cnwn_set_error("could not write itype for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
            ret = cnwn_file_write_uint16(f, entries[i].unused);
            if (ret < 0) {
                cnwn_set_error("could not write unused for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
        }
        ret = cnwn_file_seek(f, header->resources_offset);
        if (ret < 0) {
            cnwn_set_error("could not seek resources, %s", cnwn_get_error());
            return -1;
        }
        for (int i = 0; i < header->num_entries; i++) {
            ret = cnwn_file_write_uint32(f, entries[i].resource_offset);
            if (ret < 0) {
                cnwn_set_error("could not write resource offset for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
            ret = cnwn_file_write_uint32(f, entries[i].resource_size);
            if (ret < 0) {
                cnwn_set_error("could not write resource size for entry #%d, %s", i, cnwn_get_error());
                return -1;
            }
            did_bytes += ret;
        }
    }
    if (ret_bytes != NULL)
        *ret_bytes = did_bytes;
    return header->num_entries;
}

int cnwn_erf_read_localized_strings(cnwn_File * f, const cnwn_ERFHeader * header, char *** ret_strings, int64_t * ret_bytes)
{
    if (header->num_localized_strings > 0 && header->localized_strings_size > 0 && header->localized_strings_offset > 0) {
        int64_t rseek = cnwn_file_seek(f, header->localized_strings_offset);
        if (rseek < 0) {
            cnwn_set_error("could not seek localized strings, %s", cnwn_get_error());
            return -1;
        }
        // FIXME: this function is broken
        int64_t did_bytes = 0;
        int offset = 0;
        char * tmps = malloc(sizeof(char) * (header->localized_strings_size + 1));
        while (offset < header->localized_strings_size) {
            uint8_t buffer[1024];
            int64_t size = CNWN_MIN(header->localized_strings_size - offset, 1024);
            int64_t readret = cnwn_file_read(f, size, buffer);
            if (readret < 0) {
                cnwn_set_error("could not read localized strings, %s", cnwn_get_error());
                free(tmps);
                return -1;
            }
            did_bytes += readret;
            for (int i = 0; i < readret; i++) {
                if (buffer[i] > 32)
                    tmps[offset++] = buffer[i];
                else
                    tmps[offset++] = 32;
            }
        }
        tmps[offset] = 0;
        printf("* '%s' %d\n", tmps, offset);
        int num_strings = 0;
        free(tmps);
        if (ret_strings != NULL)
            *ret_strings = NULL;
        if (ret_bytes != NULL)
            *ret_bytes = did_bytes;
        return num_strings;
    }
    if (ret_strings != NULL)
        *ret_strings = NULL;
    if (ret_bytes != NULL)
        *ret_bytes = 0;
    return 0;
}
