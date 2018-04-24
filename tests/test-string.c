#include "cnwn/string.h"

void test_find(const char * s, int index, const char * sub, const char * esc)
{
    int ret = cnwn_strfind(s, index, sub, esc);
    printf("Find '%s' in '%s' => %d\n", sub, s, ret);
}

void test_rfind(const char * s, int index, const char * sub, const char * esc)
{
    int ret = cnwn_strrfind(s, index, sub, esc);
    printf("RFind '%s' in '%s' => %d\n", sub, s, ret);
}

void test_splitjoin(const char * s, int max_splits, const char * sep, const char * esc)
{
    char tmps[1024];
    char ** ret = cnwn_strsplit(s, max_splits, sep, esc);
    printf("Split: '%s' (%d)\n", s, max_splits);
    for (char ** p = ret; *p != NULL; p++)
        printf("    => '%s'\n", *p);
    int retjoin = cnwn_strjoin(tmps, sizeof(tmps), ret, ",");
    printf("Join: => '%s' (%d)\n", tmps, retjoin);
    cnwn_strings_free(ret);
}


int main(int argc, char * argv[])
{
    // test_find("ABC$$$", 5, "$", "!");
    // test_splitjoin("ABC$$$DEF", -1, "$", "!");
    // printf("%d\n", cnwn_strstartswith("ABCDEF", "AD"));
    return 0;
}
