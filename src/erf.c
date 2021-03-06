#include "cnwn/erf.h"

const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_ERF = {
    "ERF",
    {
        &cnwn_resource_init_from_file_erf, // init1
        NULL, // init2
        &cnwn_resource_deinit_erf, // deinit
        NULL, // extract
        NULL, // archive
        &cnwn_resource_get_num_meta_files_erf, // get num meta
        &cnwn_resource_get_meta_file_erf, // get meta
        &cnwn_resource_meta_file_extract_erf, // extract meta
        NULL // archive meta
    }
};

typedef struct cnwn_Entry_s {
    uint32_t id;
    uint32_t offset;
    uint32_t size;
    char key[33];
    uint16_t type;
    uint16_t unused;    
} cnwn_Entry;


cnwn_Version cnwn_erf_parse_version(const char * s)
{
    cnwn_Version ret = {0};
    if (s != NULL
        && (s[0] == 'V' || s[0] == 'v')
        && s[1] >= '0' && s[1] <= '9'
        && s[2] == '.'
        && s[3] >= '0' && s[3] <= '9'
        && ((s[4] >= '0' && s[4] <= '9') || s[4] == 0 || s[4] == 32)) {
        ret.major = (int)s[1] - 48;
        ret.major = (int)s[1] - 48;
        if (s[4] > 32) {
            ret.minor = ((int)s[3] - 48) * 10;
            ret.minor += s[4] - 48;
        } else
            ret.minor = s[3] - 48;
    }
    return ret;
}

int cnwn_resource_init_from_file_erf(cnwn_Resource * resource, cnwn_File * f)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    int64_t ret;
    ret = cnwn_file_read_string(f, 5, resource->r.r_erf.typestr);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading filetype");
        return -1;
    }
    cnwn_ResourceType rtype = cnwn_resource_type_from_path(resource->r.r_erf.typestr);
    if (!CNWN_RESOURCE_TYPE_IS_ERF(rtype)) {
        cnwn_set_error("trying to init ERF from non-ERF type (%s)", resource->r.r_erf.typestr);
        return -1;
    }
    resource->type = rtype;
    // if (resource->type != rtype) {
    //     cnwn_set_error("ERF contains different type (got %s, expected %s)", CNWN_RESOURCE_TYPE_EXTENSION(rtype), CNWN_RESOURCE_TYPE_EXTENSION(resource->type));
    //     return -1;
    // }
    ret = cnwn_file_read_string(f, 5, resource->r.r_erf.versionstr);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading version");
        return -1;
    }        
    resource->r.r_erf.version = cnwn_erf_parse_version(resource->r.r_erf.versionstr);
    if (resource->r.r_erf.version.major != 1 || (resource->r.r_erf.version.minor != 0 && resource->r.r_erf.version.minor != 1)) {
        cnwn_set_error("unsupported version (%s %d.%d)", resource->r.r_erf.versionstr, resource->r.r_erf.version.major, resource->r.r_erf.version.minor);
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
        int key_size = (resource->r.r_erf.version.minor > 0 ? 32 : 16);
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
            int ret = cnwn_resource_init_from_file(&subresource, entries[i].type, entries[i].key, resource->offset + entries[i].offset, entries[i].size, resource, f);
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

void cnwn_resource_deinit_erf(cnwn_Resource * resource)
{
}

int cnwn_resource_get_num_meta_files_erf(const cnwn_Resource * resource)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    return 2;
}

int cnwn_resource_get_meta_file_erf(const cnwn_Resource * resource, int index, cnwn_MetaFile * ret_meta_file)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    if (index == 0) {
        if (ret_meta_file != NULL) {
            cnwn_strcpy(ret_meta_file->name, sizeof(ret_meta_file->name), "erf-header", -1);
            cnwn_strcpy(ret_meta_file->description, sizeof(ret_meta_file->description), "ERF header", -1);
            ret_meta_file->size = 160;
        }
        return 1;
    } else if (index == 1) {
        if (ret_meta_file != NULL) {
            cnwn_strcpy(ret_meta_file->name, sizeof(ret_meta_file->name), "erf-strings", -1);
            cnwn_strcpy(ret_meta_file->description, sizeof(ret_meta_file->description), "ERF localized strings", -1);
            ret_meta_file->size = resource->r.r_erf.localized_strings_size;
        }
        return 1;
    }
    return 0;
}

int64_t cnwn_resource_meta_file_extract_erf(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f)
{
    if (!CNWN_RESOURCE_TYPE_IS_ERF(resource->type)) {
        cnwn_set_error("%s() type mismatch %d\n", __func__, resource->type);
        return -1;
    }
    if (index == 0) {
        int64_t ret = cnwn_file_seek(input_f, 0);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "seek");
            return -1;
        }
        ret = cnwn_file_copy(input_f, 160, output_f);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
            return -1;
        }
        return ret;
    } else if (index == 1) {
        int64_t ret = cnwn_file_seek(input_f, resource->r.r_erf.localized_strings_offset);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "seek");
            return -1;
        }
        ret = cnwn_file_copy(input_f, resource->r.r_erf.localized_strings_size, output_f);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "copy");
            return -1;
        }
        return ret;
    }
    cnwn_set_error("invalid meta file index (erf %d)", index);
    return -1;
}
