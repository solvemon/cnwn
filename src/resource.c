#include "cnwn/resource.h"

typedef struct cnwn_ERFEntry_s {
    char key[128];
    uint32_t id;
    uint16_t type;
    uint16_t unused;
    uint32_t offset;
    uint32_t size;
} cnwn_ERFEntry;

static int cnwn_resource_init_read_erf_file(cnwn_Resource * resource, cnwn_File * f)
{
    if (resource->type == CNWN_RESOURCE_TYPE_ERF
        || resource->type == CNWN_RESOURCE_TYPE_MOD
        || resource->type == CNWN_RESOURCE_TYPE_HAK) {
        uint8_t filetype[5] = {0};
        uint8_t version[5] = {0};
        uint32_t tmpu32;
        int64_t ret;
        ret = cnwn_file_read_fixed(f, 4, filetype);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading filetype");
            return -1;
        }
        if (filetype[0] == 'M' && filetype[1] == 'O' && filetype[2] == 'D')
            resource->type = CNWN_RESOURCE_TYPE_MOD;
        else if (filetype[0] == 'E' && filetype[1] == 'R' && filetype[2] == 'F')
            resource->type = CNWN_RESOURCE_TYPE_ERF;
        else if (filetype[0] == 'H' && filetype[1] == 'A' && filetype[2] == 'K')
            resource->type = CNWN_RESOURCE_TYPE_HAK;
        else {
            char tmps[5] = {0};
            for (int i = 0; i < 4; i++)
                tmps[i] = filetype[i];            
            cnwn_set_error("invalid ERF type \"%s\"", tmps);
            return -1;
        }
            
        ret = cnwn_file_read_fixed(f, 4, version);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading version");
            return -1;
        }        
        resource->r.r_erf.major_version = version[1] - 48;
        if (version[4] > 32) {
            resource->r.r_erf.minor_version = (version[3] - 48) * 10;
            resource->r.r_erf.minor_version += version[4] - 48;
        } else
            resource->r.r_erf.minor_version = version[3] - 48;
        if (version[0] != 'V' || version[2] != '.') {
            char tmps[5] = {0};
            for (int i = 0; i < 4; i++)
                tmps[i] = version[i];            
            cnwn_set_error("invalid version \"%s\"", tmps);
            return -1;
        }
        if (resource->r.r_erf.major_version != 1 || (resource->r.r_erf.minor_version != 0 && resource->r.r_erf.minor_version != 1)) {
            cnwn_set_error("%s (%s %d.%d)", cnwn_get_error(), "unsupported version", resource->r.r_erf.major_version, resource->r.r_erf.minor_version);
            return -1;
        }
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading num localized strings");
            return -1;
        }
        if (tmpu32 > INT_MAX) {
            cnwn_set_error("%s (%s %u)", cnwn_get_error(), "too many localized strings", tmpu32);
            return -1;
        }
        resource->r.r_erf.num_localized_strings = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading localized strings size");
            return -1;
        }
        resource->r.r_erf.localized_strings_size = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading num subresources");
            return -1;
        }
        if (tmpu32 > INT_MAX) {
            cnwn_set_error("%s (%s %u)", cnwn_get_error(), "too many subresources", tmpu32);
            return -1;
        }
        resource->r.r_erf.num_subresources = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading localized strings offset");
            return -1;
        }
        resource->r.r_erf.localized_strings_offset = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading keys offset");
            return -1;
        }
        resource->r.r_erf.keys_offset = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading values offset");
            return -1;
        }
        resource->r.r_erf.values_offset = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading year");
            return -1;
        }
        resource->r.r_erf.year = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading day of year");
            return -1;
        }
        resource->r.r_erf.day_of_year = cnwn_endian_ltoh32(tmpu32);
        ret = cnwn_file_readu32(f, &tmpu32);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading description strref");
            return -1;
        }        
        uint8_t tmp116[116];
        ret = cnwn_file_read_fixed(f, 116, &tmp116);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading last 116 bytes");
            return -1;
        }
        // FIXME: store this somewhere?
        if (resource->r.r_erf.num_subresources > 0) {
            ret = cnwn_file_seek(f, resource->r.r_erf.keys_offset);        
            if (ret < 0) {
                cnwn_set_error("%s (%s %"PRId64")", cnwn_get_error(), "seeking keys offset", resource->r.r_erf.keys_offset);
                return -1;
            }
            uint16_t tmpu16;
            int key_size = (resource->r.r_erf.minor_version > 0 ? 32 : 16);
            cnwn_ERFEntry * entries = malloc(sizeof(cnwn_ERFEntry) * resource->r.r_erf.num_subresources);
            memset(entries, 0, sizeof(cnwn_ERFEntry) * resource->r.r_erf.num_subresources);
            for (int i = 0; i < resource->r.r_erf.num_subresources; i++) {
                ret = cnwn_file_read_string(f, key_size + 1, entries[i].key);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d)", cnwn_get_error(), "reading key", i);
                    free(entries);
                    return -1;
                }
                if (!cnwn_resource_name_valid(entries[i].key)) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "invalid name", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                ret = cnwn_file_readu32(f, &tmpu32);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading ID", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                entries[i].id = cnwn_endian_ltoh32(tmpu32);
                ret = cnwn_file_readu16(f, &tmpu16);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading type", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                entries[i].type = cnwn_endian_ltoh16(tmpu16);
                ret = cnwn_file_readu16(f, &tmpu16);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading unused", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                entries[i].unused = cnwn_endian_ltoh16(tmpu16);
            }
            ret = cnwn_file_seek(f, resource->r.r_erf.values_offset);        
            if (ret < 0) {
                cnwn_set_error("%s (%s %"PRId64")", cnwn_get_error(), "seeking values offset", resource->r.r_erf.values_offset);
                free(entries);
                return -1;
            }
            for (int i = 0; i < resource->r.r_erf.num_subresources; i++) {
                ret = cnwn_file_readu32(f, &tmpu32);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading resource offset", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                entries[i].offset = cnwn_endian_ltoh32(tmpu32);
                ret = cnwn_file_readu32(f, &tmpu32);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading resource size", i, entries[i].key);
                    free(entries);
                    return -1;
                }
                entries[i].size = cnwn_endian_ltoh32(tmpu32);
            }
            resource->r.r_erf.subresources = malloc(sizeof(cnwn_Resource) * resource->r.r_erf.num_subresources);
            memset(resource->r.r_erf.subresources, 0, sizeof(cnwn_Resource) * resource->r.r_erf.num_subresources);
            for (int i = 0; i < resource->r.r_erf.num_subresources; i++) {
                ret = cnwn_file_seek(f, entries[i].offset);
                if (ret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "seeking resource offset", i, entries[i].key);
                    free(entries);
                    cnwn_resource_deinit(resource);
                    return -1;
                }
                int subresret = cnwn_resource_init_read_file(resource->r.r_erf.subresources + i, entries[i].type, entries[i].key, entries[i].size, f);
                if (subresret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "init resource", i, entries[i].key);
                    free(entries);
                    cnwn_resource_deinit(resource);
                    return -1;
                }
            }
            free(entries);
        }
        cnwn_string_array_append(&resource->filenames, "%s.xml", resource->entry_filename);
        return 0;
    }
    cnwn_set_error("%s() invalid resource type %d", __func__, resource->type);
    return -1;
}

bool cnwn_resource_name_valid(const char * name)
{
    return true; // FIXME!
}

int cnwn_resource_init_read_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f)
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
    if (offset < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "getting seek");
        return -1;
    }
    const cnwn_ResourceTypeInfo * info = cnwn_resource_type_info(type);
    resource->type = type;
    resource->name = cnwn_strdup(name);
    resource->offset = offset;
    resource->size = size;
    if (info != NULL && !cnwn_strisblank(info->extension)) 
        resource->entry_filename = cnwn_strformat("%s.%s", name, info->extension);
    else
        resource->entry_filename = cnwn_strdup(name);
    cnwn_string_array_init(&resource->filenames);
    int ret = 0;
    switch (type) {
    case CNWN_RESOURCE_TYPE_ERF:
    case CNWN_RESOURCE_TYPE_MOD:
    case CNWN_RESOURCE_TYPE_HAK:
        ret = cnwn_resource_init_read_erf_file(resource, f);
        break;
    default:
        cnwn_string_array_append(&resource->filenames, resource->entry_filename);
        break;
    }
    if (ret < 0)
        cnwn_resource_deinit(resource);
    return ret;
}

void cnwn_resource_deinit(cnwn_Resource * resource)
{
    switch (resource->type) {
    case CNWN_RESOURCE_TYPE_ERF:
    case CNWN_RESOURCE_TYPE_MOD:
    case CNWN_RESOURCE_TYPE_HAK:
        if (resource->r.r_erf.subresources != NULL) {
            for (int i = 0; i < resource->r.r_erf.num_subresources; i++)
                cnwn_resource_deinit(resource->r.r_erf.subresources + i);
            free(resource->r.r_erf.subresources);
        }
        break;
    default:
        break;
    }
    if (resource->name != NULL)
        free(resource->name);
    cnwn_array_deinit(&resource->filenames);
    if (resource->entry_filename != NULL)
        free(resource->entry_filename);
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

int cnwn_resource_get_num_subresources(const cnwn_Resource * resource)
{
    switch (resource->type) {
    case CNWN_RESOURCE_TYPE_ERF:
    case CNWN_RESOURCE_TYPE_MOD:
    case CNWN_RESOURCE_TYPE_HAK:
        return resource->r.r_erf.num_subresources;
    default:
        break;
    }
    return 0;
}

const cnwn_Resource * cnwn_resource_get_subresource(const cnwn_Resource * resource, int index)
{
    switch (resource->type) {
    case CNWN_RESOURCE_TYPE_ERF:
    case CNWN_RESOURCE_TYPE_MOD:
    case CNWN_RESOURCE_TYPE_HAK:
        if (index < 0)
            index += resource->r.r_erf.num_subresources;
        if (index >= 0 && index < resource->r.r_erf.num_subresources) 
            return resource->r.r_erf.subresources + index;
        break;
    default:
        break;
    }
    return NULL;
}

int cnwn_resource_get_num_filenames(const cnwn_Resource * resource)
{
    return resource->filenames.length;
}

const char * cnwn_resource_get_filename(const cnwn_Resource * resource, int index)
{
    return cnwn_string_array_get(&resource->filenames, index);
}

const char * cnwn_resource_get_entry_filename(const cnwn_Resource * resource)
{
    return resource->entry_filename;
}

int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * input_f, const char * path)
{
    int64_t seek = cnwn_file_seek(input_f, resource->offset);
    if (seek < 0) {
        cnwn_set_error("%s (seek %"PRId64")", cnwn_get_error(), resource->offset);
        return -1;
    }

    char tmps[CNWN_PATH_MAX_SIZE];
    cnwn_path_directorypart(tmps, sizeof(tmps), path);
    if (!cnwn_strisblank(tmps)) {
        if (cnwn_file_system_mkdir(path) < 0) {
            cnwn_set_error("%s (mkdir)", cnwn_get_error());
            return -1;
        }
    } 
    cnwn_File * f = cnwn_file_open(path, "w");
    if (f == NULL) {
        cnwn_set_error("%s (open)", cnwn_get_error());
        return -1;
    }
    int64_t written = cnwn_file_copy(input_f, resource->size, f);
    if (written < 0) {
        cnwn_set_error("%s (write %"PRId64")", cnwn_get_error(), resource->size);
        cnwn_file_close(f);
        return -1;
    }
    cnwn_file_close(f);
    return written;
}

static int cnwn_resource_count_all_items(const cnwn_Resource * resource)
{
    int ret = 0;
    if (resource != NULL) {
        ret += resource->filenames.length;
        int num_subs = cnwn_resource_get_num_subresources(resource);
        for (int i = 0; i < num_subs; i++)
            ret += cnwn_resource_count_all_items(cnwn_resource_get_subresource(resource, i));
    }
    return ret;
}

static int cnwn_resource_set_all_items(const cnwn_Resource * resource, cnwn_ResourceIteratorItem * items, const char * path, bool top)
{
    int ret = 0;
    if (resource != NULL) {
        int num_files = cnwn_resource_get_num_filenames(resource);
        for (int i = 0; i < num_files; i++) {
            char tmppath[CNWN_PATH_MAX_SIZE];
            const char * filename = cnwn_resource_get_filename(resource, i);
            items[i].resource = resource;
            items[i].filename_index = i;
            if (!cnwn_strisblank(path))
                snprintf(tmppath, sizeof(tmppath), "%s%s%s", path, CNWN_PATH_SEPARATOR, filename);
            else
                snprintf(tmppath, sizeof(tmppath), "%s", filename);
            items[i].path = cnwn_strdup(tmppath);
        }
        ret += num_files;
        char tmpdirpath[CNWN_PATH_MAX_SIZE];
        if (top)
            tmpdirpath[0] = 0;
        else {
            const cnwn_ResourceTypeInfo * info = cnwn_resource_type_info(resource->type);
            if (info != NULL && !cnwn_strisblank(info->extension)) {
                if (!cnwn_strisblank(path))
                    snprintf(tmpdirpath, sizeof(tmpdirpath), "%s%s%s.%s%s", path, CNWN_PATH_SEPARATOR, resource->name, info->extension, CNWN_PATH_SEPARATOR);
                else
                    snprintf(tmpdirpath, sizeof(tmpdirpath), "%s.%s%s", resource->name, info->extension, CNWN_PATH_SEPARATOR);
            } else {
                if (!cnwn_strisblank(path))
                    snprintf(tmpdirpath, sizeof(tmpdirpath), "%s%s%s%s", path, CNWN_PATH_SEPARATOR, resource->name, CNWN_PATH_SEPARATOR);
                else
                    snprintf(tmpdirpath, sizeof(tmpdirpath), "%s%s", resource->name, CNWN_PATH_SEPARATOR);
            }
        }
        int num_subs = cnwn_resource_get_num_subresources(resource);
        for (int i = 0; i < num_subs; i++) {
            ret += cnwn_resource_set_all_items(cnwn_resource_get_subresource(resource, i), items + ret, tmpdirpath, false);
        }
    }
    return ret;
}

int cnwn_resource_iterator_init(cnwn_ResourceIterator * iterator, const cnwn_Resource * resource)
{
    memset(iterator, 0, sizeof(cnwn_ResourceIterator));
    iterator->num_items = cnwn_resource_count_all_items(resource);
    if (iterator->num_items > 0) {
        iterator->items = malloc(sizeof(cnwn_ResourceIteratorItem) * iterator->num_items);
        cnwn_resource_set_all_items(resource, iterator->items, NULL, true);
    }
    return iterator->num_items;
}

void cnwn_resource_iterator_deinit(cnwn_ResourceIterator * iterator)
{
    if (iterator->items != NULL) {
        for (int i = 0; i < iterator->num_items; i++)
            if (iterator->items[i].path != NULL)
                free(iterator->items[i].path);
        free(iterator->items);
    }
}

int cnwn_resource_iterator_get_length(const cnwn_ResourceIterator * iterator)
{
    return iterator->num_items;
}

const cnwn_ResourceIteratorItem * cnwn_resource_iterator_get(const cnwn_ResourceIterator * iterator, int index)
{
    if (index < 0)
        index += iterator->num_items;
    if (index >= 0 && index < iterator->num_items)
        return iterator->items + index;
    return NULL;
}
