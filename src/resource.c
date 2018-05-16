#include "cnwn/resource.h"

cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE] = {0};

static void cnwn_resource_array_deinit_elements(void * elements, int length)
{
    cnwn_Resource * resources = elements;
    for (int i = 0; i < length; i++)
        cnwn_resource_deinit(resources + i);
}

bool cnwn_resource_name_valid(const char * name, const cnwn_Version * version)
{
    if (version == NULL)
        return true;
    else if (version->major != 1 || version->minor < 0 || version->minor > 1)
        return false;
    if (name == NULL || name[0] == 0)
        return false;
    for (int i = 0; name[i] != 0; i++)
        if ((i >= 32 && version->minor == 1) || (i >= 16 && version->minor == 0) || !CNWN_RESOURCE_NAME_CHAR_VALID(name[i]))
            return false;
    return true;
}

int cnwn_resource_init(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t offset, int64_t size, cnwn_Resource * parent)
{
    memset(resource, 0, sizeof(cnwn_Resource));
    if (!CNWN_RESOURCE_TYPE_VALID(type)) {
        cnwn_set_error("invalid resource type %d", type);
        return -1;
    }
    // if (!cnwn_resource_name_valid(name, &version)) {
    //     cnwn_set_error("invalid resource name \"%s\"", name);
    //     return -1;
    // }
    if (offset < 0) {
        cnwn_set_error("invalid offset %"PRId64, offset);
        return -1;
    }
    if (size < 0) {
        cnwn_set_error("invalid size %"PRId64, offset);
        return -1;
    }
    resource->type = type;
    resource->name = cnwn_strdup(name);
    resource->offset = offset;
    resource->size = size;
    resource->parent = parent;
    cnwn_ContainerCallbacks cb_resources = {NULL, &cnwn_resource_array_deinit_elements, NULL};
    cnwn_array_init(&resource->resources, sizeof(cnwn_Resource), &cb_resources);
    return 0;
}

int cnwn_resource_init_from_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t offset, int64_t size, cnwn_Resource * parent, cnwn_File * input_f)
{
    if (cnwn_resource_init(resource, type, name, offset, size, parent) < 0)
        return -1;
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    if (handler->callbacks.f_init_from_file != NULL) {
        if (handler->callbacks.f_init_from_file(resource, input_f) < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), name);
            cnwn_resource_deinit(resource);
            return -1;
        }
    }
    return 0;
}

int cnwn_resource_init_from_path(cnwn_Resource * resource, cnwn_Resource * parent, const char * path);

void cnwn_resource_deinit(cnwn_Resource * resource)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler != NULL && handler->callbacks.f_deinit != NULL) 
        handler->callbacks.f_deinit(resource);
    cnwn_array_deinit(&resource->resources);
    if (resource->name != NULL)
        free(resource->name);
    memset(resource, 0, sizeof(cnwn_Resource));
}

cnwn_ResourceType cnwn_resource_get_type(const cnwn_Resource * resource)
{
    return resource->type;
}
    
cnwn_ResourceTypeInfo cnwn_resource_get_type_info(const cnwn_Resource * resource)
{
    return CNWN_RESOURCE_TYPE_INFO(resource->type);
}

const char * cnwn_resource_get_name(const cnwn_Resource * resource)
{
    return resource->name != NULL ? resource->name : "";
}

int cnwn_resource_get_path(const cnwn_Resource * resource, int max_size, char * ret_path)
{
    char filename[CNWN_PATH_MAX_SIZE];
    if (resource->parent != NULL && resource->parent->parent != NULL) {
        char parentpath[CNWN_PATH_MAX_SIZE];
        cnwn_resource_get_path(resource->parent, sizeof(parentpath), parentpath);
        snprintf(filename, sizeof(filename), "%s%s%s.%s", parentpath, CNWN_PATH_SEPARATOR, resource->name, CNWN_RESOURCE_TYPE_EXTENSION(resource->type));
    } else
        snprintf(filename, sizeof(filename), "%s.%s", resource->name, CNWN_RESOURCE_TYPE_EXTENSION(resource->type));
    return cnwn_strcpy(ret_path, max_size, filename, -1);
}

int cnwn_resource_get_num_resources(const cnwn_Resource * resource)
{
    return cnwn_array_get_length(&resource->resources);
}

cnwn_Resource * cnwn_resource_get_resource(const cnwn_Resource * resource, int index)
{
    return cnwn_array_element_ptr(&resource->resources, index);
}

int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f)
{
    if (cnwn_file_seek(input_f, resource->offset) < 0) {
        cnwn_set_error("%s (seek)", cnwn_get_error());
        return -1;
    }
    int64_t ret;
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    if (handler->callbacks.f_extract != NULL)
        ret = handler->callbacks.f_extract(resource, input_f, output_f);
    else
        ret = cnwn_file_copy(input_f, resource->size, output_f);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), resource->name);
        return -1;
    }
    return ret;
}

int64_t cnwn_resource_extract_to_path(const cnwn_Resource * resource, cnwn_File * input_f, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_path_directorypart(tmps, sizeof(tmps), path);
    if (!cnwn_strisblank(tmps)) {
        int r = cnwn_file_system_mkdir(tmps);
        if (r < 0)
            return -1;
    }
    cnwn_File * output_f = cnwn_file_open(path, "wt");
    if (output_f == NULL) {
        cnwn_set_error("%s \"%s\"", cnwn_get_error(), path);
        return -1;
    }
    int64_t ret = cnwn_resource_extract(resource, input_f, output_f);
    cnwn_file_close(output_f);
    return ret;
}

int64_t cnwn_resource_archive(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f)
{
    if (cnwn_file_seek(input_f, resource->offset) < 0) {
        cnwn_set_error("%s (seek)", cnwn_get_error());
        return -1;
    }
    int64_t ret;
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    if (handler != NULL && handler->callbacks.f_archive != NULL)
        ret = handler->callbacks.f_archive(resource, input_f, output_f);
    else
        ret = cnwn_file_copy(input_f, resource->size, output_f);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), resource->name);
        return -1;
    }
    return ret;
}

int cnwn_resource_get_num_meta_files(const cnwn_Resource * resource)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler != NULL && handler->callbacks.f_get_num_meta_files != NULL)
        return handler->callbacks.f_get_num_meta_files(resource);
    return 0;
}

int cnwn_resource_get_meta_file(const cnwn_Resource * resource, int index, cnwn_MetaFile * ret_meta_file)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler != NULL && handler->callbacks.f_get_meta_file != NULL)
        return handler->callbacks.f_get_meta_file(resource, index, ret_meta_file);
    if (ret_meta_file != NULL)
        memset(ret_meta_file, 0, sizeof(cnwn_MetaFile));
    return 0;
}

int64_t cnwn_resource_meta_file_extract(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    int num_meta_files = cnwn_resource_get_num_meta_files(resource);
    int use_index = index;
    if (use_index < 0)
        use_index += num_meta_files;
    if (use_index >= 0 && use_index < num_meta_files) {
        int64_t ret = 0;
        if (handler->callbacks.f_meta_file_extract != NULL)
            ret = handler->callbacks.f_meta_file_extract(resource, use_index, input_f, output_f);
        if (ret < 0) 
            cnwn_set_error("%s (%s)", cnwn_get_error(), resource->name);
        return ret;
    }
    cnwn_set_error("invalid meta file index (%d)", index);
    return -1;
}

int64_t cnwn_resource_meta_file_extract_to_path(const cnwn_Resource * resource, int index, cnwn_File * input_f, const char * path)
{
    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_path_directorypart(tmps, sizeof(tmps), path);
    if (!cnwn_strisblank(tmps)) {
        int r = cnwn_file_system_mkdir(tmps);
        if (r < 0)
            return -1;
    }
    cnwn_File * output_f = cnwn_file_open(path, "wt");
    if (output_f == NULL) {
        cnwn_set_error("%s \"%s\"", cnwn_get_error(), path);
        return -1;
    }
    int64_t ret = cnwn_resource_meta_file_extract(resource, index, input_f, output_f);
    cnwn_file_close(output_f);
    return ret;
}

int64_t cnwn_resource_meta_file_archive(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    int64_t ret = 0;
    if (handler->callbacks.f_meta_file_archive != NULL)
        ret = handler->callbacks.f_meta_file_archive(resource, index, input_f, output_f);
    if (ret < 0) 
        cnwn_set_error("%s (%s)", cnwn_get_error(), resource->name);        
    return ret;
}

