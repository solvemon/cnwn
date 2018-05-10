#include "cnwn/resource.h"

const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_NONE = {0};

cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE] = {0};

static void cnwn_resource_array_deinit_elements(void * elements, int length)    
{
    cnwn_Resource * resources = elements;
    for (int i = 0; i < length; i++)
        cnwn_resource_deinit(resources + i);
}

bool cnwn_resource_name_valid(const char * name)
{
    return true; // FIXME!
}

int cnwn_resource_init_extract(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f)
{
    memset(resource, 0, sizeof(cnwn_Resource));
    if (type <= CNWN_RESOURCE_TYPE_INVALID || type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid resource type %d", type);
        return -1;
    }
    if (!cnwn_resource_name_valid(name)) {
        cnwn_set_error("invalid resource name \"%s\"", name);
        return -1;
    }
    if (size < 0) {
        cnwn_set_error("negative size");
        return -1;
    }
    int64_t offset = cnwn_file_get_seek(f);
    if (offset < 0) 
        return -1;
    resource->type = type;
    resource->name = cnwn_strdup(name);
    resource->offset = offset;
    resource->size = size;
    cnwn_ContainerCallbacks cb = {NULL, cnwn_resource_array_deinit_elements, NULL};
    cnwn_array_init(&resource->resources, sizeof(cnwn_Resource), &cb);
    int ret = 0;
    if (CNWN_RESOURCE_HANDLER(type).f_init_extract != NULL)
        ret = CNWN_RESOURCE_HANDLER(type).f_init_extract(resource, size, f);
    if (ret < 0)
        cnwn_resource_deinit(resource);
    for (int i = 0; i < resource->resources.length; i++)
        ((cnwn_Resource *)resource->resources.data)[i].parent = resource;
    return ret;
}

int cnwn_resource_init_archive(cnwn_Resource * resource, cnwn_ResourceType type, const char * name)
{
    memset(resource, 0, sizeof(cnwn_Resource));
    if (type <= CNWN_RESOURCE_TYPE_INVALID || type >= CNWN_MAX_RESOURCE_TYPE) {
        cnwn_set_error("invalid resource type %d", type);
        return -1;
    }
    if (!cnwn_resource_name_valid(name)) {
        cnwn_set_error("invalid resource name \"%s\"", name);
        return -1;
    }    
    resource->type = type;
    resource->name = cnwn_strdup(name);
    cnwn_ContainerCallbacks cb = {NULL, cnwn_resource_array_deinit_elements, NULL};
    cnwn_array_init(&resource->resources, sizeof(cnwn_Resource), &cb);
    int ret = 0;
    if (CNWN_RESOURCE_HANDLER(type).f_init_archive != NULL)
        ret = CNWN_RESOURCE_HANDLER(type).f_init_archive(resource);
    if (ret < 0)
        cnwn_resource_deinit(resource);
    for (int i = 0; i < resource->resources.length; i++)
        ((cnwn_Resource *)resource->resources.data)[i].parent = resource;
    return ret;
}

void cnwn_resource_deinit(cnwn_Resource * resource)
{
    if (CNWN_RESOURCE_HANDLER(resource->type).f_deinit != NULL)
        CNWN_RESOURCE_HANDLER(resource->type).f_deinit(resource);
    if (resource->name != NULL)
        free(resource->name);
    cnwn_array_deinit(&resource->resources);
    memset(resource, 0, sizeof(cnwn_Resource));
}


cnwn_ResourceType cnwn_resource_get_type(const cnwn_Resource * resource)
{
    return (resource->type > CNWN_RESOURCE_TYPE_INVALID && resource->type < CNWN_MAX_RESOURCE_TYPE ? resource->type : CNWN_RESOURCE_TYPE_INVALID);
}

const char * cnwn_resource_get_name(const cnwn_Resource * resource)
{
    return (resource->name != NULL ? resource->name : "");
}

int cnwn_resource_get_filename(const cnwn_Resource * resource, int max_size, char * ret_filename)
{
    const cnwn_ResourceTypeInfo * info = cnwn_resource_type_info(resource->type);    
    char tmps[CNWN_PATH_MAX_SIZE];
    if (info != NULL && !cnwn_strisblank(info->extension)) 
        snprintf(tmps, sizeof(tmps), "%s.%s", resource->name, info->extension);
    else
        snprintf(tmps, sizeof(tmps), "%s", resource->name);
    return cnwn_strcpy(ret_filename, max_size, tmps, -1);
}

int cnwn_resource_get_num_items(const cnwn_Resource * resource)
{
    if (CNWN_RESOURCE_HANDLER(resource->type).f_get_num_items != NULL)
        return CNWN_RESOURCE_HANDLER(resource->type).f_get_num_items(resource);
    return 0;
}

int cnwn_resource_get_item(const cnwn_Resource * resource, int index, cnwn_ResourceItem * ret_item)
{
    int num_items = cnwn_resource_get_num_items(resource);
    if (index < 0)
        index += num_items;
    if (index < 0 || index >= num_items) 
        return 0;
    if (CNWN_RESOURCE_HANDLER(resource->type).f_get_item != NULL)
        return CNWN_RESOURCE_HANDLER(resource->type).f_get_item(resource, index, ret_item);
    return 0;
}

int64_t cnwn_resource_extract_item(const cnwn_Resource * resource, int index, cnwn_File * source_f, cnwn_File * destination_f)
{
    int num_items = cnwn_resource_get_num_items(resource);
    if (index < 0)
        index += num_items;
    if (index < 0 || index >= num_items) {
        cnwn_set_error("invalid item index %d", index);
        return -1;
    }
    if (CNWN_RESOURCE_HANDLER(resource->type).f_extract_item != NULL)
        return CNWN_RESOURCE_HANDLER(resource->type).f_extract_item(resource, index, source_f, destination_f);
    return cnwn_file_copy(source_f, resource->size, destination_f);
}

int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * source_f, cnwn_File * destination_f)
{
    int64_t ret = cnwn_file_seek(source_f, resource->offset);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "seek");
        return -1;
    }
    if (CNWN_RESOURCE_HANDLER(resource->type).f_extract != NULL)
        return CNWN_RESOURCE_HANDLER(resource->type).f_extract(resource, source_f, destination_f);
    ret = cnwn_file_copy(source_f, resource->size, destination_f);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
        return -1;
    }
    return ret;
}

int cnwn_resource_get_num_resources(const cnwn_Resource * resource)
{
    return cnwn_array_get_length(&resource->resources);
}

cnwn_Resource * cnwn_resource_get_resource(const cnwn_Resource * resource, int index)
{
    return cnwn_array_element_ptr(&resource->resources, index);
}
