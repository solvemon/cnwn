#include "cnwn/erf.h"

void dump_resource(const cnwn_Resource * resource, int indent)
{
    for (int i = 0; i < indent; i++) printf(" ");
    char path[CNWN_PATH_MAX_SIZE];
    cnwn_resource_get_path(resource, sizeof(path), path); 
    int num_meta_files = cnwn_resource_get_num_meta_files(resource);
    printf("Resource '%s' %"PRId64" %d meta_files\n", path, resource->size, num_meta_files);
    for (int j = 0; j < num_meta_files; j++) {
        for (int i = 0; i < indent; i++) printf(" ");
        cnwn_MetaFile meta_file;
        if (cnwn_resource_get_meta_file(resource, j, &meta_file) > 0)
            printf("+Meta %d '%s'\n", j, meta_file.name);
    }
    int num_resources = cnwn_resource_get_num_resources(resource);
    for (int j = 0; j < num_resources; j++) {
        cnwn_Resource * subresource = cnwn_resource_get_resource(resource, j);
        dump_resource(subresource, indent + 4);
    }
}

void extract_resource(const cnwn_Resource * resource, int depth, cnwn_File * input_f, const char * destination_path)
{
    char resource_path[CNWN_PATH_MAX_SIZE];
    cnwn_resource_get_path(resource, sizeof(resource_path), resource_path);
    int num_meta_files = cnwn_resource_get_num_meta_files(resource);
    for (int j = 0; j < num_meta_files; j++) {
        cnwn_MetaFile meta_file;
        if (cnwn_resource_get_meta_file(resource, j, &meta_file) > 0) {
            char meta_file_path[CNWN_PATH_MAX_SIZE];
            if (resource->parent == NULL) 
                cnwn_strcat(meta_file_path, sizeof(meta_file_path), CNWN_PATH_SEPARATOR, destination_path, meta_file.name, NULL);
            else
                cnwn_strcat(meta_file_path, sizeof(meta_file_path), CNWN_PATH_SEPARATOR, destination_path, resource_path, meta_file.name, NULL);
            int64_t rs = cnwn_resource_meta_file_extract_to_path(resource, j, input_f, meta_file_path);
            if (rs >= 0) 
                printf("Meta %s => %s %"PRId64"\n", meta_file.name, meta_file_path, rs);
            else
                fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
        }
    }
    int num_resources = cnwn_resource_get_num_resources(resource);
    if (depth != 0 && num_resources > 0) {
        for (int j = 0; j < num_resources; j++) {
            cnwn_Resource * subresource = cnwn_resource_get_resource(resource, j);
            extract_resource(subresource, (depth > 0 ? depth - 1 : -1), input_f, destination_path);
        }
    } else {
        char tmps[CNWN_PATH_MAX_SIZE];
        cnwn_strcat(tmps, sizeof(tmps), CNWN_PATH_SEPARATOR, destination_path, resource_path, NULL);
        int64_t rs = cnwn_resource_extract_to_path(resource, input_f, tmps);
        if (rs >= 0) 
            printf("Extracted %s => %s %"PRId64"\n", resource->name, tmps, rs);
        else
            fprintf(stderr, "ERROR: %s\n", cnwn_get_error());
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
            int ret = cnwn_resource_init_from_file(&resource, CNWN_RESOURCE_TYPE_MOD, "test", 0, size, NULL, f);
            if (ret >= 0) {
                printf("Initialized resource.\n");
                // dump_resource(&resource, 0);
                extract_resource(&resource, -1, f, "./tmp");
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
