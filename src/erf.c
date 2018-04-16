#include "cnwn/erf.h"

int cnwn_erf_read_contents(int fd, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries)
{
    cnwn_ERFHeader tmpheader = {0};
    cnwn_ERFHeader * header = (ret_header != NULL ? ret_header : &tmpheader);
    uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
    char tmps[128];
    int64_t ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header type (%s)", cnwn_get_error());
        return -1;
    }
    for (int64_t i = 0; i < ret && i < 4; i++)
        header->type_str[i] = data[i];
    cnwn_resource_key_clean(header->type_str, 128, tmps);
    header->type = cnwn_resource_type_from_extension(tmps);
    if (header->type <= CNWN_RESOURCE_TYPE_NONE || header->type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid resource type (%s)", header->type_str);
        return -1;
    }
    ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) {
        cnwn_set_error("error reading header version (%s)", cnwn_get_error());
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
        cnwn_set_error("invalid version (%s)", header->version_str);
        return -1;
    }
    if (header->version.major != 1 || header->version.minor > 1) {
        cnwn_set_error("unsupported version (%d.%d)", header->version.major, header->version.minor);
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings count (%s)", cnwn_get_error());
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings size (%s)", cnwn_get_error());
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->num_entries);
    if (ret < 0) {
        cnwn_set_error("error reading header entries count (%s)", cnwn_get_error());
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header localized strings offset (%s)", cnwn_get_error());
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->keys_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header keys offset (%s)", cnwn_get_error());
        return -1;
    }
    ret = cnwn_read_uint32(fd, &header->resources_offset);
    if (ret < 0) {
        cnwn_set_error("error reading header resources offset (%s)", cnwn_get_error());
        return -1;
    }
    if (header->num_localized_strings > 0 && header->localized_strings_offset < 160) {
        cnwn_set_error("invalid localized strings offset (%u)", header->localized_strings_offset);
        return -1;
    }
    if (header->num_entries > 0 && header->keys_offset < 160) {
        cnwn_set_error("invalid keys offset (%u)", header->keys_offset);
        return -1;
    }
    if (header->num_entries > 0 && header->resources_offset < 160) {
        cnwn_set_error("invalid resources offset (%u)", header->resources_offset);
        return -1;
    }
    int num_entries = header->num_entries;
    max_entries = CNWN_MAX(0, max_entries);
    if (max_entries > 0)
        num_entries = CNWN_MIN(num_entries, max_entries);
    else if (ret_entries != NULL)
        return 0;
    if (ret_entries != NULL && num_entries > 0) {
        ret = cnwn_seek(fd, header->keys_offset);
        if (ret < 0) {
            cnwn_set_error("error seeking keys offset (%s)", cnwn_get_error());
            return -1;
        }
        int key_size = (header->version.minor > 0 ? 32 : 16);
        for (int i = 0; i < num_entries; i++) {
            ret_entries[i].key_offset = header->keys_offset + i * (key_size + 8);
            ret = cnwn_read_bytes(fd, key_size, data);
            if (ret < 0) {
                cnwn_set_error("error reading key for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
            if (ret < key_size) {
                cnwn_set_error("not enough data to read key for entry #%d (%"PRId64")", i, ret);
                return -1;
            }
            int soffset = 0;
            for (int j = 0; j < ret; j++)
                if (data[j] > 32)
                    ret_entries[i].key[soffset++] = data[j];
            ret_entries[i].key[soffset] = 0;
            ret = cnwn_read_uint32(fd, &ret_entries[i].resource_id);
            if (ret < 0) {
                cnwn_set_error("error reading resource ID for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
            ret = cnwn_read_uint16(fd, &ret_entries[i].erf_type);
            if (ret < 0) {
                cnwn_set_error("error reading ERF type for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
            ret_entries[i].type = cnwn_resource_type_from_erf_type(ret_entries[i].erf_type);
            if (ret_entries[i].type <= CNWN_RESOURCE_TYPE_NONE || ret_entries[i].type >= CNWN_MAX_RESOURCE_TYPE) {
                cnwn_set_error("invalid ERF resource type for entry #%d (%u)", i, ret_entries[i].erf_type);
                return -1;
            }
            ret = cnwn_read_uint16(fd, &ret_entries[i].unused);
            if (ret < 0) {
                cnwn_set_error("error reading unused for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
        }
        ret = cnwn_seek(fd, header->resources_offset);
        if (ret < 0) {
            cnwn_set_error("error seeking resources offset (%s)", cnwn_get_error());
            return -1;
        }
        for (int i = 0; i < num_entries; i++) {
            ret = cnwn_read_uint32(fd, &ret_entries[i].resource_offset);
            if (ret < 0) {
                cnwn_set_error("error reading resource offset for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
            if (ret_entries[i].resource_offset < header->resources_offset) {
                cnwn_set_error("invalid resource offset for entry #%d (%u)", i, ret_entries[i].resource_offset);
                return -1;
            }
            ret = cnwn_read_uint32(fd, &ret_entries[i].resource_size);
            if (ret < 0) {
                cnwn_set_error("error reading resource size for entry #%d (%s)", i, cnwn_get_error());
                return -1;
            }
        }
    }
    return num_entries;
}

int cnwn_erf_read_contents_path(const char * path, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        cnwn_set_error("error opening \"%s\" (%s)", path, strerror(errno));
        return -1;
    }
    int ret = cnwn_erf_read_contents(fd, regexps, ret_header, max_entries, ret_entries);
    close(fd);
    return ret;
}

int cnwn_erf_read_contents_path_fd(const char * path, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries, int * ret_fd)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        cnwn_set_error("error opening \"%s\" (%s)", path, strerror(errno));
        return -1;
    }
    int ret = cnwn_erf_read_contents(fd, regexps, ret_header, max_entries, ret_entries);
    if (ret_fd != NULL)
        *ret_fd = fd;
    else
        close(fd);
    return ret;
}

int cnwn_erf_entry_extract(int fd, uint32_t offset, uint32_t size, int output_fd)
{
    int retbytes = 0;
    if (size > 0) {
        int64_t ret = cnwn_seek(fd, offset);
        if (ret < 0) {
            cnwn_set_error("error seeking entry offset (%s)", cnwn_get_error());
            return -1;
        }
        uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
        for (uint32_t i = 0; i < size / CNWN_READ_WRITE_BUFFER_SIZE; i++) {
            ret = cnwn_read_bytes(fd, CNWN_READ_WRITE_BUFFER_SIZE, data);
            if (ret < 0) {
                cnwn_set_error("error reading entry (%s)", cnwn_get_error());
                return -1;
            }
            ret = cnwn_write_bytes(output_fd, ret, data);
            if (ret < 0) {
                cnwn_set_error("error writing entry (%s)", cnwn_get_error());
                return -1;
            }
            retbytes += ret;
        }
        if (size % CNWN_READ_WRITE_BUFFER_SIZE) {
            ret = cnwn_read_bytes(fd, size % CNWN_READ_WRITE_BUFFER_SIZE, data);
            if (ret < 0) {
                cnwn_set_error("error reading entry (%s)", cnwn_get_error());
                return -1;
            }
            ret = cnwn_write_bytes(output_fd, ret, data);
            if (ret < 0) {
                cnwn_set_error("error writing entry (%s)", cnwn_get_error());
                return -1;
            }
            retbytes += ret;
        }
    }
    return retbytes;
}

int cnwn_erf_entry_extract_path(int fd, uint32_t offset, uint32_t size, const char * output_path)
{
    int output_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
    if (output_fd < 0) {
        cnwn_set_error("error opening \"%s\" (%s)", output_path, strerror(errno));
        return -1;
    }
    int ret = cnwn_erf_entry_extract(fd, offset, size, output_fd);
    close(output_fd);
    return ret;
}

int cnwn_erf_entry_extract_path2(const char * path, uint32_t offset, uint32_t size, const char * output_path)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd < 0) {
        cnwn_set_error("error opening \"%s\" (%s)", path, strerror(errno));
        return -1;
    }
    int ret = cnwn_erf_entry_extract_path(fd, offset, size, output_path);
    close(fd);
    return ret;
}
