#include "cnwn/file.h"

void test_parts(const char * path) {
    char dirs[CNWN_PATH_MAX_SIZE], base[CNWN_PATH_MAX_SIZE], fn[CNWN_PATH_MAX_SIZE], ext[CNWN_PATH_MAX_SIZE];
    cnwn_path_dirpart(dirs, sizeof(dirs), path);
    cnwn_path_basepart(base, sizeof(base), path);
    cnwn_path_filepart(fn, sizeof(fn), path);
    cnwn_path_extpart(ext, sizeof(ext), path);
    printf("Path: %s => (%s) (%s) (%s) (%s)\n", path, dirs, base, fn, ext);
}


int main(int argc, char * argv[])
{
    test_parts("/a/b/c/d.txt");
    char ** ret = cnwn_listdir(NULL, true, NULL, false);
    if (ret == NULL)
        printf("ERROR: %s\n", cnwn_get_error());
    for (char ** p = ret; p != NULL && *p != NULL; p++)
        printf("Content: %s\n", *p);
    cnwn_free_strings(ret);
    return 0;
}
