#include "cnwn/file.h"

int main(int argc, char * argv[])
{
    const char * path = "/dir1//dir2/dir3/filename.txt";
    char tmps[1024];
    cnwn_path_filename(path, sizeof(tmps), tmps);
    printf("Filename '%s'\n", tmps);
    cnwn_path_directory(path, sizeof(tmps), tmps);
    printf("Directory '%s'\n", tmps);
    cnwn_path_extension(path, sizeof(tmps), tmps);
    printf("Extension '%s'\n", tmps);
    path = "this\\/is\\/escaped\\\\\\doubleslash";
    cnwn_path_unescape(path, sizeof(tmps), tmps);
    printf("Unescape '%s'\n", tmps);
    if (cnwn_mkdirs("test1/test2/test3") < 0)
        printf("ERROR: %s\n", cnwn_get_error());
    return 0;
}
