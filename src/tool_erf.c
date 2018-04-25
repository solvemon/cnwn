#include "cnwn/tool_erf.h"

int cnwn_tool_erf_command_info(const cnwn_ToolErfSettings * settings, const char * path)
{
    if (cnwn_strisblank(path)) {
        fprintf(stderr, "ERROR: invalid empty path\n");
        return 1;
    }
    cnwn_ErfHeader * header = NULL;
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        return 1;
    }
    int64_t ret = cnwn_erf_header_read(f, &header);
    if (ret < 0) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_file_close(f);
        return 1;
    }
    printf("ERF %s %d.%d, %d strings (offset=%"PRId64", size=%"PRId64"), %d entries (koffset=%"PRId64", voffset=%"PRId64", size=%"PRId64")\n",
           CNWN_ERF_TYPE_STRING(header->type),
           header->version.major, header->version.minor,
           header->num_string_entries,
           header->internal.string_entries_offset,
           header->internal.string_entries_size,
           header->num_resource_entries,
           header->internal.keys_offset,
           header->internal.values_offset,
           cnwn_erf_header_sum_resources(header));
    if (settings != NULL && settings->verbose) {
        printf("String entries:\n");
        for (int i = 0; i < header->num_string_entries; i++) {
            int64_t ret = cnwn_file_seek(f, header->string_entries[i].offset);
            if (ret < 0) {
                fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
                cnwn_file_close(f);
                return 1;
            }
            char tmps[64];
            int64_t rsize = CNWN_MIN(header->string_entries[i].size, sizeof(tmps));
            ret = cnwn_file_read_string(f, rsize, tmps);
            if (ret < 0) {
                fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
                cnwn_file_close(f);
                return 1;
            }
            cnwn_strnoctl(tmps, sizeof(tmps), tmps);
            printf("  %d = \"%s\"%s (%"PRId64")\n", i, tmps, header->string_entries[i].size > 63 ? "+" : "", header->string_entries[i].size);
        }
        printf("Resource entries:\n");
        for (int i = 0; i < header->num_resource_entries; i++) {
            printf("  %s (size=%"PRId64" offset=%"PRId64" key=%"PRId64" value=%"PRId64")\n",
                   header->resource_entries[i].filename,
                   header->resource_entries[i].size,
                   header->resource_entries[i].offset,
                   header->resource_entries[i].internal.key_offset,
                   header->resource_entries[i].internal.value_offset
                   );
        }
    }
    cnwn_file_close(f);
    cnwn_erf_header_free(header);
    return 0;
}

int cnwn_tool_erf_command_list(const cnwn_ToolErfSettings * settings, const char * path, int argc, char * argv[])
{
    if (cnwn_strisblank(path)) {
        fprintf(stderr, "ERROR: invalid empty path\n");
        return 1;
    }
    for (int i = 0; i < argc; i++) {
    }
    cnwn_ErfHeader * header = NULL;
    cnwn_File * f = cnwn_file_open(path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        return 1;
    }
    int64_t ret = cnwn_erf_header_read(f, &header);
    if (ret < 0) {
        fprintf(stderr, "ERROR: %s \"%s\"\n", cnwn_get_error(), path);
        cnwn_file_close(f);
        return 1;
    }
    for (int i = 0; i < header->num_resource_entries; i++) {
        if (settings != NULL && settings->verbose) {
            printf("%s %"PRId64" (offset=%"PRId64")\n",
                   header->resource_entries[i].filename,
                   header->resource_entries[i].size,
                   header->resource_entries[i].offset);
        } else
            printf("%s\n", header->resource_entries[i].filename);
    }
    cnwn_file_close(f);
    cnwn_erf_header_free(header);
    return 0;
}
