#include "cnwn/resource.h"

int main(int argc, char * argv[])
{
    printf("Type: %d\n", cnwn_resource_type_from_extension("something.mod"));
    char tmps[1024];
    cnwn_ResourceType rt;
    cnwn_resource_type_and_key_to_filename(CNWN_RESOURCE_TYPE_MOD, "something", sizeof(tmps), tmps);
    printf("Filename: %s\n", tmps);
    cnwn_resource_type_and_key_from_filename("/a/b/c/something.mod", &rt, sizeof(tmps), tmps);
    printf("Type: %d, key: %s\n", rt, tmps);
    printf("Valid key: %d\n", cnwn_resource_key_valid("abc09_/asd"));
    return 0;
}
