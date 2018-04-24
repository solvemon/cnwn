#include "cnwn/file_system.h"

void test_ls(const char * path, bool recurse)
{
    char ** list = cnwn_file_system_ls(path, recurse);
    if (list != NULL) {
        printf("List:\n");
        for (char ** p = list; *p != NULL; p++)
            printf("    => '%s'\n", *p);
        cnwn_strings_free(list);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
}

void test_mkdir(const char * path)
{
    int ret = cnwn_file_system_mkdir(path);
    if (ret >= 0) {
        printf("Mkdir: '%s' => %d\n", path, ret);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
}

void test_rm(const char * path)
{
    int ret = cnwn_file_system_rm(path);
    if (ret >= 0) {
        printf("Rm: '%s' => %d\n", path, ret);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
}

void test_mv(const char * path, const char * to_path)
{
    int ret = cnwn_file_system_mv(path, to_path);
    if (ret >= 0) {
        printf("Mv: '%s' to '%s' => %d\n", path, to_path, ret);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
}

int main(int argc, char * argv[])
{
    // test_ls(NULL, true);
    test_mkdir("tmp1/tmp2/tmp3");
    // test_rm("tmp1");
    test_mv("tmp1", "../build/tmp1");
    printf("File size: %"PRId64"\n", cnwn_file_system_size("CMakeCache.txt", true));
    printf("File size: %"PRId64"\n", cnwn_file_system_size(".", true));
    return 0;
}
