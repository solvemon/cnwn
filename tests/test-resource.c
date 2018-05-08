#include "cnwn/resource.h"

int main(int argc, char * argv[])
{
    cnwn_Resource resource;
    cnwn_File * f = cnwn_file_open(argc > 1 ? argv[1] : "../tests/test.mod", "r");
    if (f != NULL) {
        int64_t size = cnwn_file_size(f);
        if (size >= 0) {
            int ret = cnwn_resource_init_read_file(&resource, CNWN_RESOURCE_TYPE_ERF, "test", size, f);
            if (ret >= 0) {
                printf("Initialized resource!\n");
                cnwn_ResourceIterator iterator;
                cnwn_resource_iterator_init(&iterator, &resource);
                int length = cnwn_resource_iterator_get_length(&iterator);
                printf("Iterator length: %d\n", length);
                for (int i = 0; i < length; i++) {
                    const cnwn_ResourceIteratorItem * item = cnwn_resource_iterator_get(&iterator, i);
                    int64_t ret = cnwn_resource_extract(item->resource, f, item->path);
                    if (ret >= 0) 
                        printf("%d Wrote '%s' => '%s' %"PRId64"\n", i, item->resource->name, item->path, ret);
                    else
                        fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
                }
                cnwn_resource_iterator_deinit(&iterator);
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
