#include "cnwn/erf.h"

int main(int argc, char * argv[])
{
    cnwn_File * f = cnwn_file_open_r(argc > 1 ? argv[1] : "../tests/test-01.mod");
    if (f != NULL) {
        cnwn_ERFHeader header;
        cnwn_ERFEntry * entries;
        int ret = cnwn_erf_read_header(f, &header, &entries);
        if (ret >= 0) {
            char tmps[1024];
            cnwn_erf_header_to_string(&header, true, sizeof(tmps), tmps);
            printf("ERF: %s\n", tmps);
            for (int i = 0; i < ret; i++) {
                cnwn_erf_entry_to_string(entries + i, true, sizeof(tmps), tmps);
                printf("  %s\n", tmps);
            }
            free(entries);
        } else
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        cnwn_file_close(f);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
    return 0;
}
