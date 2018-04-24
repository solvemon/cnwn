#include "cnwn/erf.h"

int64_t cnwn_erf_read_header(cnwn_File * f, cnwn_ErfInfo * ret_info, int max_string_entries, cnwn_ErfStringEntry * ret_string_entries, int max_resource_entries, cnwn_ErfResourceEntry * ret_resource_entries)
{
    if (ret_info != NULL)
        memset(ret_info, 0, sizeof(cnwn_ErfInfo));
    if (ret_string_entries != NULL && max_string_entries > 0)
        memset(ret_string_entries, 0, sizeof(cnwn_ErfStringEntry) * max_string_entries);
    if (ret_resource_entries != NULL && max_resource_entries > 0)
        memset(ret_resource_entries, 0, sizeof(cnwn_ErfResourceEntry) * max_resource_entries);
    uint8_t buffer[CNWN_FILE_BUFFER_SIZE];
    int64_t ret = 0;
    int64_t ioret;
    ioret = cnwn_file_read_fixed(f, 4, buffer);
    if (ioret < 0) {
        cnwn_set_error("%s (erf type)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    if (ret_info != NULL) {
        for (int i = 0; i < ioret; i++)
            ret_info->internal.type_str[i] = (buffer[i] > 32 ? buffer[i] : ' ');
        ret_info->internal.type_str[ioret] = 0;
        if (cnwn_strncmpi(ret_info->internal.type_str, "MOD", 3) == 0)
            ret_info->type = CNWN_ERF_TYPE_MODULE;
    }
    ioret = cnwn_file_read_fixed(f, 4, buffer);
    if (ioret < 0) {
        cnwn_set_error("%s (erf version)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    if (ret_info != NULL) {
        for (int i = 0; i < ioret; i++)
            ret_info->internal.version_str[i] = (buffer[i] > 32 ? buffer[i] : ' ');
        ret_info->internal.version_str[ioret] = 0;
        if ((ret_info->internal.version_str[0] == 'V' || ret_info->internal.version_str[0] == 'v')
            && ret_info->internal.version_str[1] >= '0' && ret_info->internal.version_str[1] <= '9'
            && ret_info->internal.version_str[2] == '.'
            && ret_info->internal.version_str[3] >= '0' && ret_info->internal.version_str[3] <= '9') {
            ret_info->version.major = ret_info->internal.version_str[1] - '0';
            ret_info->version.minor = ret_info->internal.version_str[1] - '0';
        }
    }
    return ret;
}

int64_t cnwn_erf_write_header(cnwn_File * f, const cnwn_ErfInfo * info, int num_string_entries, const cnwn_ErfStringEntry * string_entries, int num_resource_entries, const cnwn_ErfResourceEntry * resource_entries)
{
    return 0;
}
