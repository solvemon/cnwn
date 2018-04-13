#include "cnwn.h"

int main(int argc, char * argv[])
{
    cnwn_ERFHeader header = {0};
    cnwn_ERFEntry entries[1024] = {0};
    int ret = cnwn_erf_read_path(argc > 1 ? argv[1] : "../tests/test-01.mod", &header, 1024, entries);
    printf("Returned: %d (%s)\n", ret, ret < 0 ? cnwn_get_error() : "");
    printf("Header type: %s (%s)\n", CNWN_RESOURCE_TYPE_EXTENSION(header.type), header.type_str);
    printf("Header version: %d.%d (%s)\n", header.version.major, header.version.minor, header.version_str);
    printf("Localized strings: %u size=%u offset=%u\n", header.num_localized_strings, header.localized_strings_size, header.localized_strings_offset);
    printf("Entries: %u keys_offset=%u resources_offset=%u\n", header.num_entries, header.keys_offset, header.resources_offset);
    for (int i = 0; i < ret; i++) {
        printf("  Entry: %s type=%s key_offset=%u resource_offset=%u resource_id=%u resource_size=%u\n",
               entries[i].key,
               CNWN_RESOURCE_TYPE_EXTENSION(entries[i].type),
               entries[i].key_offset,
               entries[i].resource_offset,
               entries[i].resource_id,
               entries[i].resource_size);
        
    }
    // char key[1024];
    // cnwn_ResourceType resource_type;
    // ret = cnwn_resource_from_filename("abc/def/geh/test this thing asdasdh aksjhda skjdh.res", &resource_type, sizeof(key), key);
    // printf("%d '%s' %d\n", ret, key, resource_type);
    // ret = cnwn_resource_to_filename(CNWN_RESOURCE_TYPE_RES, "a simple test asdasd as dasd +123123213123", sizeof(key), key);
    // printf("'%s'\n", key);
    return 0;
}
