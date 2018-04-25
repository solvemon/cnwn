#include "cnwn/restypes/erf.h"


int main(int argc, char * argv[])
{
    cnwn_File * f = cnwn_file_open(argc > 1 ? argv[1] : "../tests/test.mod", "r");
    cnwn_ErfHeader * header = NULL;
    int64_t ret = cnwn_erf_header_read(f, &header);
    printf("Read returned: %"PRId64"\n", ret);
    if (header != NULL) {
        printf("ERF type: %s\n", CNWN_ERF_TYPE_STRING(header->type));
        printf("ERF version: %d.%d\n", header->version.major, header->version.minor);
        printf("String entries: %d\n", header->num_string_entries);
        printf("Resource entries: %d\n", header->num_resource_entries);
        printf("  String entries offset: %"PRId64"\n", header->internal.string_entries_offset);
        printf("  String entries size: %"PRId64"\n", header->internal.string_entries_size);
        printf("  Keys offset: %"PRId64"\n", header->internal.keys_offset);
        printf("  Values offset: %"PRId64"\n", header->internal.values_offset);
        printf("String entries (%d):\n", header->num_string_entries);
        for (int i = 0; i < header->num_string_entries; i++) {
            if (cnwn_file_seek(f, header->string_entries[i].offset) >= 0) {
                char tmps[1024];
                int64_t size = CNWN_MIN(header->string_entries[i].size + 1, sizeof(tmps));
                int sret = cnwn_file_read_string(f, size, tmps);
                if (sret >= 0) 
                    printf("  %d = '%s'\n", i, tmps);
                else
                    fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
            } else
                fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        }
        printf("Resource entries (%d):\n", header->num_resource_entries);
        for (int i = 0; i < header->num_resource_entries; i++) {
            printf("  %d = %s offset=%"PRId64", size=%"PRId64"\n",
                   i,
                   header->resource_entries[i].filename,
                   header->resource_entries[i].offset,
                   header->resource_entries[i].size);
        }
        printf("Resources total size: %"PRId64"\n", cnwn_erf_header_sum_resources(header));
        cnwn_erf_header_free(header);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
    cnwn_file_close(f);
    return 0;
}
