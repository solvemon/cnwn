#include "cnwn/erf.h"

void dump_resources(const cnwn_Resource * resource, int indent)
{
    for (int i = 0; i < indent; i++) printf(" ");
    int num_items = cnwn_resource_get_num_items(resource);
    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_resource_get_filename(resource, sizeof(tmps), tmps);
    printf("Resource '%s' %"PRId64" %d items\n", tmps, resource->size, num_items);
    for (int j = 0; j < num_items; j++) {
        for (int i = 0; i < indent; i++) printf(" ");
        cnwn_ResourceItem item;
        cnwn_resource_get_item(resource, j, &item);
        printf("+Item %d '%s'\n", j, item.filename);
    }
    int num_resources = cnwn_resource_get_num_resources(resource);
    for (int j = 0; j < num_resources; j++) {
        cnwn_Resource * subresource = cnwn_resource_get_resource(resource, j);
        dump_resources(subresource, indent + 4);
    }
}

void extract_resources(const cnwn_Resource * resource, cnwn_File * source_f, int depth, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    char usepath[CNWN_PATH_MAX_SIZE];
    char filename[CNWN_PATH_MAX_SIZE];
    cnwn_strcpy(usepath, sizeof(usepath), path, -1);
    cnwn_resource_get_filename(resource, sizeof(tmps), tmps);
    if (cnwn_strisblank(usepath))
        snprintf(filename, sizeof(filename), "%s", tmps);
    else
        snprintf(filename, sizeof(filename), "%s%s%s", path, CNWN_PATH_SEPARATOR, tmps);
    int num_resources = cnwn_resource_get_num_resources(resource);
    if (depth == 0 || !CNWN_RESOURCE_TYPE_IS_CONTAINER(resource->type)) {
        cnwn_File * destination_f = cnwn_file_open(filename, "w");
        if (destination_f != NULL) {
            int64_t ret = cnwn_resource_extract(resource, source_f, destination_f);
            if (ret >= 0)
                printf("Extracted %s => %s %"PRId64"\n", resource->name, filename, ret);
            else
                fprintf(stderr, "ERROR: %s: %s\n", cnwn_get_error(), filename);
            cnwn_file_close(destination_f);
        } else
            fprintf(stderr, "ERROR: %s: %s", cnwn_get_error(), filename);
    } else {
        if (num_resources > 0 && resource->parent != NULL) {
            if (cnwn_file_system_mkdir(filename) >= 0)
                printf("Mkdir '%s'\n", filename);
            else
                fprintf(stderr, "ERROR: %s: %s\n", cnwn_get_error(), filename);
            cnwn_strappend(usepath, sizeof(usepath), usepath, "%s%s", CNWN_PATH_SEPARATOR, filename);
        }
        int num_items = cnwn_resource_get_num_items(resource);
        for (int i = 0; i < num_items; i++) {
            cnwn_ResourceItem item;
            cnwn_resource_get_item(resource, i, &item);
            cnwn_strappend(filename, sizeof(filename), usepath, "%s%s", CNWN_PATH_SEPARATOR, item.filename);
            cnwn_File * destination_f = cnwn_file_open(filename, "w");
            if (destination_f != NULL) {
                int64_t ret = cnwn_resource_extract_item(resource, i, source_f, destination_f);
                if (ret >= 0)
                    printf("Item '%s' %"PRId64"\n", filename, ret);
                else
                    fprintf(stderr, "ERROR: %s: %s\n", cnwn_get_error(), filename);
                cnwn_file_close(destination_f);
            } else
                fprintf(stderr, "ERROR: %s: %s\n", cnwn_get_error(), filename);
        }
        // for (int i = 0; i < num_resources; i++) {
        //     cnwn_Resource * subresource = cnwn_resource_get_resource(resource, i);
        //     extract_resources(subresource, source_f, (depth > 0 ? depth - 1 : -1), usepath);
        // }
    }
}

int main(int argc, char * argv[])
{

    CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_ERF] = CNWN_RESOURCE_HANDLER_ERF;
    CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_MOD] = CNWN_RESOURCE_HANDLER_ERF;
    CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_HAK] = CNWN_RESOURCE_HANDLER_ERF;
    CNWN_RESOURCE_HANDLERS[CNWN_RESOURCE_TYPE_NWM] = CNWN_RESOURCE_HANDLER_ERF;
    
    cnwn_Resource resource;
    cnwn_File * f = cnwn_file_open(argc > 1 ? argv[1] : "../tests/test.mod", "r");
    if (f != NULL) {
        int64_t size = cnwn_file_size(f);
        if (size >= 0) {
            int ret = cnwn_resource_init_extract(&resource, CNWN_RESOURCE_TYPE_ERF, "test", size, f);
            if (ret >= 0) {
                printf("Initialized resource!\n");
                //dump_resources(&resource, 0);
                extract_resources(&resource, f, -1, "./tmp");
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
