#include "cnwn.h"

int cnwn_erf_read(int fd, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries)
{
    int64_t original_seek = cnwn_seek_cur(fd);
    if (original_seek < 0) {
        cnwn_set_error("Could not get current offset, %s", cnwn_get_error());
        return -1;
    }
    cnwn_ERFHeader header = {0};
    uint8_t data[CNWN_READ_WRITE_BUFFER_SIZE];
    char tmps[128];
    int64_t ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) 
        return -1;
    for (int64_t i = 0; i < ret && i < 4; i++)
        header.type_str[i] = data[i];
    cnwn_clean_string(header.type_str, 128, tmps);
    header.type = cnwn_resource_type_from_extension(tmps);
    ret = cnwn_read_bytes(fd, 4, data);
    if (ret < 0) 
        return -1;
    for (int64_t i = 0; i < ret && i < 4; i++)
        header.version_str[i] = data[i];
    if ((header.version_str[0] == 'V' || header.version_str[0] == 'v')
        && header.version_str[1] >= '0' && header.version_str[1] <= '9'
        && header.version_str[2] == '.'
        && header.version_str[3] >= '0' && header.version_str[3] <= '9') {
        header.version.major = (int)header.version_str[1] - (int)'0';
        header.version.minor = (int)header.version_str[3] - (int)'0';
    }
    ret = cnwn_seek(fd, original_seek);
    if (ret < 0)
        return -1;
    
    if (ret_header != NULL)
        *ret_header = header;
    return 0;
}

