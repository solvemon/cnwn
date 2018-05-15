#include "cnwn/resource.h"

cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE] = {0};

static void cnwn_resource_array_deinit_elements(void * elements, int length)
{
    cnwn_Resource * resources = elements;
    for (int i = 0; i < length; i++)
        cnwn_resource_deinit(resources + i);
}

static void cnwn_meta_file_array_deinit_elements(void * elements, int length)
{
    cnwn_MetaFile * meta_files = elements;
    for (int i = 0; i < length; i++)
        cnwn_meta_file_deinit(meta_files + i);
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

void cnwn_meta_file_init(cnwn_MetaFile * meta_file, const char * name, const char * description, int64_t size)
{
    memset(meta_file, 0, sizeof(cnwn_MetaFile));
    meta_file->name = cnwn_strdup(name);
    meta_file->description = cnwn_strdup(description);
    meta_file->size = CNWN_MAX(0, size);
}

void cnwn_meta_file_deinit(cnwn_MetaFile * meta_file)
{
    if (meta_file->name != NULL)
        free(meta_file->name);
    if (meta_file->description != NULL)
        free(meta_file->description);
    memset(meta_file, 0, sizeof(cnwn_MetaFile));
}

const char * cnwn_meta_file_get_name(const cnwn_MetaFile * meta_file)
{
    return meta_file != NULL && meta_file->name != NULL ? meta_file->name : "";
}

const char * cnwn_meta_file_get_description(const cnwn_MetaFile * meta_file)
{
    return meta_file != NULL && meta_file->description != NULL ? meta_file->description : "";
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
    cnwn_ContainerCallbacks cb_meta_files = {NULL, &cnwn_meta_file_array_deinit_elements, NULL};
    cnwn_array_init(&resource->resources, sizeof(cnwn_Resource), &cb_meta_files);    
    return 0;
}

int cnwn_resource_init_from_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_Resource * parent, cnwn_File * input_f)
{
    int64_t offset = cnwn_file_get_seek(input_f);
    if (offset < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "seek");
        return -1;
    }
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
    cnwn_array_deinit(&resource->meta_files);
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

const char * cnwn_resource_get_path(const cnwn_Resource * resource)
{
    return resource->path != NULL ? resource->path : "";
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
    return cnwn_array_get_length(&resource->meta_files);
}

const cnwn_MetaFile * cnwn_resource_get_meta_file(const cnwn_Resource * resource, int index)
{
    return cnwn_array_element_ptr(&resource->meta_files, index);
}

int64_t cnwn_resource_meta_file_extract(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f)
{
    const cnwn_ResourceHandler * handler = CNWN_RESOURCE_HANDLER(resource->type);
    if (handler == NULL) {
        cnwn_set_error("invalid type when getting handler (%s)", resource->name);
        return -1;
    }
    int64_t ret = 0;
    if (handler->callbacks.f_meta_file_extract != NULL)
        ret = handler->callbacks.f_meta_file_extract(resource, index, input_f, output_f);
    if (ret < 0) 
        cnwn_set_error("%s (%s)", cnwn_get_error(), resource->name);        
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

