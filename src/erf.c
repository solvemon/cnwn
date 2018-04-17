#include "cnwn/erf.h"

// static int64_t cnwn_erf_default_handler(const cnwn_ERFHandlers * handlers, cnwn_ERFHandlerMode mode, const char * path, int fd, const char * output_path, int output_fd, const cnwn_ERFHeader * header, const cnwn_ERFEntry * entry)
// {
//     if (mode == CNWN_ERF_HANDLER_MODE_EXTRACT) {
//         if (entry->resource_size > 0) 
//             return cnwn_copy_bytes(fd, output_fd, entry->resource_size);
//         return 0;
//     } else if (mode == CNWN_ERF_HANDLER_MODE_ARCHIVE) {
//         return 0;
//     }
//     cnwn_set_error("invalid mode (%d)", mode);
//     return -1;
// }

// const cnwn_ERFHandler CNWN_ERF_DEFAULT_HANDLER = cnwn_erf_default_handler;


int cnwn_erf_read_header(cnwn_File * f, cnwn_ERFHeader * ret_header, cnwn_ERFEntry ** ret_entries)
{
    if (f == NULL) {
        cnwn_set_error("file is NULL");
        return -1;
    }
    cnwn_ERFHeader header = {0};
    uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
    int64_t ret = cnwn_file_read(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header type, %s", cnwn_get_error());
        return -1;
    }
    for (int64_t i = 0; i < ret && i < 4; i++)
        header.type_str[i] = data[i];
    header.type = cnwn_resource_type_from_extension(header.type_str);
    if (header.type <= CNWN_RESOURCE_TYPE_NONE || header.type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid resource type '%s'", header.type_str);
        return -1;
    }
    ret = cnwn_file_read(f, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header version, %s", cnwn_get_error());
        return -1;
    }
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
    if (header.version.major != 1 || header.version.minor > 1) {
        cnwn_set_error("unsupported version %d.%d", header.version.major, header.version.minor);
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings count, %s", cnwn_get_error());
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings size, %s", cnwn_get_error());
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.num_entries);
    if (ret < 0) {
        cnwn_set_error("error reading header entries count, %s", cnwn_get_error());
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings offset, %s", cnwn_get_error());
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.keys_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header keys offset, %s", cnwn_get_error());
        return -1;
    }
    ret = cnwn_file_read_uint32(f, &header.resources_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header resources offset, %s", cnwn_get_error());
        return -1;
    }
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
            entries[i].key_offset = header.keys_offset + i * (key_size + 8);
            ret = cnwn_file_read(f, key_size, data);
            if (ret < 0) {
                cnwn_set_error("error reading key for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
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
            ret = cnwn_file_read_uint16(f, &entries[i].erf_type);
            if (ret < 0) {
                cnwn_set_error("error reading ERF type for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
            entries[i].type = cnwn_resource_type_from_erf_type(entries[i].erf_type);
            if (entries[i].type <= CNWN_RESOURCE_TYPE_NONE || entries[i].type >= CNWN_MAX_RESOURCE_TYPE) {
                cnwn_set_error("invalid ERF resource type for entry #%d (%u)", i, entries[i].erf_type);
                free(entries);
                return -1;
            }
            ret = cnwn_file_read_uint16(f, &entries[i].unused);
            if (ret < 0) {
                cnwn_set_error("error reading unused for entry #%d (%s)", i, cnwn_get_error());
                free(entries);
                return -1;
            }
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
    return header.num_entries;
}

int cnwn_erf_list(const char * path, bool details, FILE * output)
{
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry * entries = NULL;
    cnwn_File * f = cnwn_file_open_r(path);
    if (f == NULL) {
        cnwn_set_error("could not open ERF (%s): %s\n", cnwn_get_error(), path);
        return -1;
    }
    int ret = cnwn_erf_read_header(f, &header, &entries);
    if (ret < 0) {
        cnwn_set_error("could not open ERF (%s): %s\n", cnwn_get_error(), path);
        return -1;
    }
    cnwn_file_close(f);
    if (details)
        printf("ERF: %s (%s) %d.%d (%s)\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.type_str, header.version.major, header.version.minor, header.version_str);
    int64_t totbytes = 0;
    if (entries != NULL) {
        for (int i = 0; i < ret; i++) {
            char tmps[CNWN_PATH_MAX_SIZE];
            cnwn_resource_type_and_key_to_filename(entries[i].type, entries[i].key, sizeof(tmps), tmps);
            if (details)
                printf("%s (%u bytes at offset %u)\n", tmps, entries[i].resource_size, entries[i].resource_offset);
            else
                printf("%s\n", tmps);
            totbytes += entries[i].resource_size;
        }
        free(entries);
    }
    if (details)
        printf("Total %"PRId64" bytes in %d entries.\n", totbytes, ret);
    return ret + (details ? 2 : 0);
}
