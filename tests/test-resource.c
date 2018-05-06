#include "cnwn/resource.h"

int main(int argc, char * argv[])
{
    cnwn_Resource resource;
    cnwn_File * f = cnwn_file_open(argc > 1 ? argv[1] : "../tests/test.mod", "r");
    if (f != NULL) {
        int64_t size = cnwn_file_size(f);
        if (size >= 0) {
            int ret = cnwn_resource_init_from_file(&resource, CNWN_RESOURCE_TYPE_ERF, "test", size, f);
            if (ret >= 0) {
                printf("Initialized resource!\n");
                for (int i = 0; i < cnwn_resource_get_num_subresources(&resource); i++) {
                    const cnwn_Resource * subresource = cnwn_resource_get_subresource(&resource, i);
                    printf("#%d = %d '%s'\n", i, cnwn_resource_get_type(subresource), cnwn_resource_get_name(subresource));
                }                     
                cnwn_resource_deinit(&resource);
            } else
                fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        } else
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        cnwn_file_close(f);
    } else
        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
    
    return 0;
}
