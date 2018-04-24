#include "cnwn/erf.h"

void test_read(const char * path)
{
    cnwn_File * f = cnwn_file_open_r(path);
    if (f != NULL) {
        cnwn_ERFHeader header;
        cnwn_ERFEntry * entries;
        int ret = cnwn_erf_read_header(f, &header, &entries, NULL);
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
}

void test_write(const char * path, const char * dst_path)
{
    cnwn_File * f = cnwn_file_open_r(path);
    if (f != NULL) {
        cnwn_ERFHeader header;
        cnwn_ERFEntry * entries;
        int ret = cnwn_erf_read_header(f, &header, &entries, NULL);
        if (ret >= 0) {
            cnwn_File * out_f = cnwn_file_open_w(dst_path);
            if (f != NULL) {
                int ret2 = cnwn_erf_write_header(out_f, &header, ret, entries, NULL);
                if (ret2 < 0)
                    fprintf(stderr, "ERROR: %s %s\n", cnwn_get_error(), dst_path);
                cnwn_file_close(out_f);
            } else
                fprintf(stderr, "ERROR: %s %s\n", cnwn_get_error(), dst_path);
            free(entries);
        } else
            fprintf(stderr, "ERROR: %s %s\n", cnwn_get_error(), path);
        cnwn_file_close(f);
    } else
        fprintf(stderr, "ERROR: %s %s\n", cnwn_get_error(), path);
}

int main(int argc, char * argv[])
{
    // test_read(argc > 1 ? argv[1] : "../tests/test.mod");
    test_write(argc > 1 ? argv[1] : "../tests/test.mod", "a.mod");
    test_read("a.mod");
    return 0;
}
