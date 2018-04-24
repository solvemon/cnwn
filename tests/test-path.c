#include "cnwn/path.h"

void test_clean(const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    int ret = cnwn_path_clean(tmps, sizeof(tmps), path);
    printf("'%s' => '%s' (%d)\n", path, tmps, ret);
}

void test_parts(const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE], tmps2[CNWN_PATH_MAX_SIZE];
    int ret;
    printf("'%s'\n", path);
    ret = cnwn_path_directorypart(tmps, sizeof(tmps), path);
    printf("    directory => '%s' (%d)\n", tmps, ret);
    ret = cnwn_path_basepart(tmps, sizeof(tmps), path);
    printf("    base => '%s' (%d)\n", tmps, ret);
    ret = cnwn_path_filenamepart(tmps, sizeof(tmps), path);
    printf("    filename => '%s' (%d)\n", tmps, ret);
    ret = cnwn_path_extensionpart(tmps, sizeof(tmps), path);
    printf("    extension => '%s' (%d)\n", tmps, ret);
    ret = cnwn_path_extensionparts(path, sizeof(tmps), tmps, sizeof(tmps2), tmps2);
    printf("    extension parts => '%s' and '%s' (%d)\n", tmps, tmps2, ret);
}


int main(int argc, char * argv[])
{
    test_clean("tmp1");
    test_clean("filename.ext");
    test_clean("directory1");
    test_clean("tmp1/tmp2/tmp3");
    test_clean("/directory1/directory2/filename.ext");
    test_clean("/directory1/directory2/");
    test_clean("////directory1///directory2///filename.ext");
    test_clean("////directory1\\/\\/\\/directory2///");
    test_clean("////directory1\\/\\/\\/directory2///filename.ext");
    test_parts("/directory1/directory2/filename");
    return 0;
}
