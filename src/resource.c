#include "cnwn/resource.h"

typedef struct cnwn_ERFEntry_s {
    char key[128];
    uint32_t id;
    uint16_t type;
    uint16_t unused;
    uint32_t offset;
    uint32_t size;
} cnwn_ERFEntry;

static int cnwn_resource_init_from_erf_file(cnwn_Resource * resource, cnwn_File * f)
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
        ret = cnwn_file_read_fixed(f, 4, version);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading version");
            return -1;
        }
        resource->r.r_erf.major_version = version[1] - 48;
        resource->r.r_erf.minor_version = version[3] - 48;
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
                int subresret = cnwn_resource_init_from_file(resource->r.r_erf.subresources + i, entries[i].type, entries[i].key, entries[i].size, f);
                if (subresret < 0) {
                    cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "init resource", i, entries[i].key);
                    free(entries);
                    cnwn_resource_deinit(resource);
                    return -1;
                }
            }
            free(entries);
        }
        return 0;
    }
    cnwn_set_error("%s() invalid resource type %d", __func__, resource->type);
    return -1;
}

bool cnwn_resource_name_valid(const char * name)
{
    return true; // FIXME!
}

int cnwn_resource_init_from_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f)
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
    resource->type = type;
    resource->name = cnwn_strdup(name);
    resource->offset = offset;
    resource->size = size;
    int ret = 0;
    switch (type) {
    case CNWN_RESOURCE_TYPE_ERF:
    case CNWN_RESOURCE_TYPE_MOD:
    case CNWN_RESOURCE_TYPE_HAK:
        ret = cnwn_resource_init_from_erf_file(resource, f);
        break;
    default:
        break;
    }
    if (ret < 0 && resource->name != NULL)
        free(resource->name);
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
    cnwn_strings_free(resource->filenames);
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

char ** cnwn_resource_get_filenames(const cnwn_Resource * resource)
{
    return NULL;
}
