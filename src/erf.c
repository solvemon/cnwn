#include "cnwn/erf.h"

const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_ERF = {
    cnwn_resource_init_extract_erf,
    cnwn_resource_init_archive_erf,
    cnwn_resource_deinit_erf,
    cnwn_resource_get_num_items_erf,
    cnwn_resource_get_item_erf,
    cnwn_resource_extract_item_erf,
    NULL
};

typedef struct cnwn_Entry_s {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    char key[33];
    uint16_t type;
    uint16_t unused;    
} cnwn_Entry;

int cnwn_resource_init_extract_erf(cnwn_Resource * resource, int64_t size, cnwn_File * f)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    int64_t ret;
    ret = cnwn_file_read_string(f, 5, resource->r.r_erf.erf_type);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading filetype");
        return -1;
    }
    if (resource->r.r_erf.erf_type[0] == 'M' && resource->r.r_erf.erf_type[1] == 'O' && resource->r.r_erf.erf_type[2] == 'D')
        resource->r.r_erf.type = CNWN_RESOURCE_TYPE_MOD;
    else if (resource->r.r_erf.erf_type[0] == 'E' && resource->r.r_erf.erf_type[1] == 'R' && resource->r.r_erf.erf_type[2] == 'F')
        resource->r.r_erf.type = CNWN_RESOURCE_TYPE_ERF;
    else if (resource->r.r_erf.erf_type[0] == 'H' && resource->r.r_erf.erf_type[1] == 'A' && resource->r.r_erf.erf_type[2] == 'K')
        resource->r.r_erf.type = CNWN_RESOURCE_TYPE_HAK;
    else if (resource->r.r_erf.erf_type[0] == 'N' && resource->r.r_erf.erf_type[1] == 'W' && resource->r.r_erf.erf_type[2] == 'M')
        resource->r.r_erf.type = CNWN_RESOURCE_TYPE_NWM;
    else {
        cnwn_set_error("invalid ERF type \"%s\"", resource->r.r_erf.erf_type);
        return -1;
    }
    resource->type = resource->r.r_erf.type;
    ret = cnwn_file_read_string(f, 5, resource->r.r_erf.erf_version);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading version");
        return -1;
    }        
    resource->r.r_erf.major_version = resource->r.r_erf.erf_version[1] - 48;
    if (resource->r.r_erf.erf_version[4] > 32) {
        resource->r.r_erf.minor_version = (resource->r.r_erf.erf_version[3] - 48) * 10;
        resource->r.r_erf.minor_version += resource->r.r_erf.erf_version[4] - 48;
    } else
        resource->r.r_erf.minor_version = resource->r.r_erf.erf_version[3] - 48;
    if (resource->r.r_erf.erf_version[0] != 'V' || resource->r.r_erf.erf_version[2] != '.') {
        cnwn_set_error("invalid version \"%s\"", resource->r.r_erf.erf_version);
        return -1;
    }
    if (resource->r.r_erf.major_version != 1 || (resource->r.r_erf.minor_version != 0 && resource->r.r_erf.minor_version != 1)) {
        cnwn_set_error("%s (%s %d.%d)", cnwn_get_error(), "unsupported version", resource->r.r_erf.major_version, resource->r.r_erf.minor_version);
        return -1;
    }
    ret = cnwn_file_readu32(f, &resource->r.r_erf.num_localized_strings);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading num localized strings");
        return -1;
    }
    resource->r.r_erf.num_localized_strings = cnwn_endian_ltoh32(resource->r.r_erf.num_localized_strings);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.localized_strings_size);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading localized strings size");
        return -1;
    }
    resource->r.r_erf.localized_strings_size = cnwn_endian_ltoh32(resource->r.r_erf.localized_strings_size);
    uint32_t num_entries = 0;
    ret = cnwn_file_readu32(f, &num_entries);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading num entries");
        return -1;
    }
    num_entries = cnwn_endian_ltoh32(num_entries);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.localized_strings_offset);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading localized strings offset");
        return -1;
    }
    resource->r.r_erf.localized_strings_offset = cnwn_endian_ltoh32(resource->r.r_erf.localized_strings_offset);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.keys_offset);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading keys offset");
        return -1;
    }
    resource->r.r_erf.keys_offset = cnwn_endian_ltoh32(resource->r.r_erf.keys_offset);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.values_offset);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading values offset");
        return -1;
    }
    resource->r.r_erf.values_offset = cnwn_endian_ltoh32(resource->r.r_erf.values_offset);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.year);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading year");
        return -1;
    }
    resource->r.r_erf.year = cnwn_endian_ltoh32(resource->r.r_erf.year);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.day_of_year);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading day of year");
        return -1;
    }
    resource->r.r_erf.day_of_year = cnwn_endian_ltoh32(resource->r.r_erf.day_of_year);
    ret = cnwn_file_readu32(f, &resource->r.r_erf.description_strref);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading description strref");
        return -1;
    }
    resource->r.r_erf.description_strref = cnwn_endian_ltoh32(resource->r.r_erf.description_strref);
    ret = cnwn_file_read_fixed(f, 116, resource->r.r_erf.rest);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading last 116 bytes");
        return -1;
    }
    if (num_entries > 0) {
        ret = cnwn_file_seek(f, resource->offset + resource->r.r_erf.keys_offset);        
        if (ret < 0) {
            cnwn_set_error("%s (%s %u)", cnwn_get_error(), "seeking keys offset", resource->r.r_erf.keys_offset);
            return -1;
        }
        int key_size = (resource->r.r_erf.minor_version > 0 ? 32 : 16);
        cnwn_Entry * entries = malloc(sizeof(cnwn_Entry) * num_entries);
        memset(entries, 0, sizeof(cnwn_Entry) * num_entries);
        for (int i = 0; i < num_entries; i++) {
            ret = cnwn_file_read_string(f, key_size + 1, entries[i].key);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d)", cnwn_get_error(), "reading key", i);
                free(entries);
                return -1;
            }
            // if (!cnwn_resource_name_valid(entries[i].key)) {
            //     cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "invalid name", i, entries[i].key);
            //     cnwn_erf_deinit(erf);
            //     return -1;
            // }
            ret = cnwn_file_readu32(f, &entries[i].id);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading ID", i, entries[i].key);
                free(entries);
                return -1;
            }
            entries[i].id = cnwn_endian_ltoh32(entries[i].id);
            ret = cnwn_file_readu16(f, &entries[i].type);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading type", i, entries[i].key);
                free(entries);
                return -1;
            }
            entries[i].type = cnwn_endian_ltoh16(entries[i].type);
            ret = cnwn_file_readu16(f, &entries[i].unused);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading unused", i, entries[i].key);
                free(entries);
                return -1;
            }
            entries[i].unused = cnwn_endian_ltoh16(entries[i].unused);
        }
        ret = cnwn_file_seek(f, resource->offset + resource->r.r_erf.values_offset);        
        if (ret < 0) {
            cnwn_set_error("%s (%s %u)", cnwn_get_error(), "seeking values offset", resource->r.r_erf.values_offset);
            free(entries);
            return -1;
        }
        for (int i = 0; i < num_entries; i++) {
            ret = cnwn_file_readu32(f, &entries[i].offset);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading resource offset", i, entries[i].key);
                free(entries);
                return -1;
            }
            entries[i].offset = cnwn_endian_ltoh32(entries[i].offset);
            ret = cnwn_file_readu32(f, &entries[i].size);
            if (ret < 0) {
                cnwn_set_error("%s (%s #%d \"%s\")", cnwn_get_error(), "reading resource size", i, entries[i].key);
                free(entries);
                return -1;
            }
            entries[i].size = cnwn_endian_ltoh32(entries[i].size);
        }
        for (int i = 0; i < num_entries; i++) {
            cnwn_Resource subresource;
            int ret = cnwn_resource_init_extract(&subresource, entries[i].type, entries[i].key, entries[i].size, f);
            if (ret < 0) {
                cnwn_set_error("%s (%s \"%s\")", cnwn_get_error(), "subresource", entries[i].key);
                free(entries);
                return -1;
            }
            cnwn_array_append(&resource->resources, 1, &subresource);
        }
        free(entries);
    }
    return 0;
}

int cnwn_resource_init_archive_erf(cnwn_Resource * resource)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    
    return -1;
}

void cnwn_resource_deinit_erf(cnwn_Resource * resource)
{
}

int cnwn_resource_get_num_items_erf(const cnwn_Resource * resource)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    return 3;
}

int cnwn_resource_get_item_erf(const cnwn_Resource * resource, int index, cnwn_ResourceItem * ret_item)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    if (index == 0) {
        if (ret_item != NULL) {
            snprintf(ret_item->filename, sizeof(ret_item->filename), "type.info");
            ret_item->size = 4;
        }
        return 1;
    } else if (index == 1) {
        if (ret_item != NULL) {
            snprintf(ret_item->filename, sizeof(ret_item->filename), "version.info");
            ret_item->size = 4;
        }
        return 1;
    } else if (index == 2) {
        if (ret_item != NULL) {
            snprintf(ret_item->filename, sizeof(ret_item->filename), "strings.info");
            ret_item->size = resource->r.r_erf.localized_strings_size;
        }
        return 1;
    }
    return 0;
}

int64_t cnwn_resource_extract_item_erf(const cnwn_Resource * resource, int index, cnwn_File * source_f, cnwn_File * destination_f)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    int64_t retbytes = 0;
    int64_t ret;
    if (index == 0) {
        char tmps[128] = {0};
        const cnwn_ResourceTypeInfo * info = cnwn_resource_type_info(resource->type);
        snprintf(tmps, sizeof(tmps), "%s\n", info->extension);
        cnwn_strupper(tmps, sizeof(tmps), tmps);
        printf("%s %d\n", tmps, resource->type);
        ret = cnwn_file_write_string(destination_f, tmps);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
            return -1;
        }
        retbytes += ret;
    } else if (index == 1) {
        char tmps[128] = {0};
        snprintf(tmps, sizeof(tmps), "V%d.%d\n", resource->r.r_erf.major_version, resource->r.r_erf.minor_version);
        cnwn_strupper(tmps, sizeof(tmps), tmps);
        ret = cnwn_file_write_string(destination_f, tmps);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
            return -1;
        }
        retbytes += ret;
    } else if (index == 2) {
        ret = cnwn_file_seek(source_f, resource->offset + resource->r.r_erf.localized_strings_offset);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "seek");
            return -1;
        }
        ret = cnwn_file_copy(source_f, resource->r.r_erf.localized_strings_size, destination_f);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
            return -1;
        }
        retbytes += ret;
    }
    return retbytes;
}
