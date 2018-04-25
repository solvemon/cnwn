#include "cnwn/erf.h"

const char * CNWN_ERF_TYPE_STRINGS[CNWN_MAX_ERF_TYPE] = {
    "",
    "module"
};

static void cnwn_erf_header_deinit(cnwn_ErfHeader * header)
{
    if (header != NULL) {
        if (header->string_entries != NULL)
            free(header->string_entries);
        if (header->resource_entries != NULL)
            for (int i = 0; i < header->num_resource_entries; i++) {
                if (header->resource_entries[i].key != NULL)
                    free(header->resource_entries[i].key);
                if (header->resource_entries[i].filename != NULL)
                    free(header->resource_entries[i].filename);
            }
        free(header->resource_entries);
    }
}

cnwn_ErfType cnwn_erf_type_parse(const char * s)
{
    if (s != NULL) {
        char tmps[5] = {0};
        int offset = 0;
        for (int i = 0; s[i] != 0 && i < 4; i++)
            if (s[i] > 32)
                tmps[offset++] = s[i];
        if (cnwn_strcmpi(tmps, "mod") == 0)
            return CNWN_ERF_TYPE_MODULE;
    }
    return CNWN_ERF_TYPE_NONE;
}

cnwn_ErfVersion cnwn_erf_version_parse(const char * s)
{
    cnwn_ErfVersion ret = {0};
    if (s != NULL) {
        if ((s[0] == 'V' || s[0] == 'v')
            && s[1] >= '0' && s[1] <= '9'
            && s[2] == '.'
            && s[3] >= '0' && s[3] <= '9') {
            ret.major = s[1] - '0';
            ret.minor = s[3] - '0';
        }
    }
    return ret;
}

bool cnwn_erf_version_support(const cnwn_ErfVersion * version)
{
    if (version != NULL
        && version->major == 1
        && (version->minor == 0 || version->minor == 1))
        return true;
    return false;
}


int64_t cnwn_erf_header_read_top(cnwn_File * f, cnwn_ErfHeader * ret_header)
{
    uint8_t buffer[128];
    int64_t ioret;
    int64_t ret = 0;
    uint32_t tmp32;
    // Type
    ioret = cnwn_file_read_fixed(f, 4, buffer);
    if (ioret < 0) {
        cnwn_set_error("%s (erf type)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    for (int i = 0; i < ioret && i < 4; i++)
        ret_header->internal.type_str[i] = (buffer[i] > 32 ? buffer[i] : ' ');
    ret_header->type = cnwn_erf_type_parse(ret_header->internal.type_str);
    if (!CNWN_ERF_TYPE_VALID(ret_header->type)) {
        char tmps[128];
        cnwn_strescape(tmps, sizeof(tmps), ret_header->internal.type_str, "\"", "\\");
        cnwn_set_error("invalid ERF type \"%s\"", tmps);
        return -1;
    }
    // Version
    ioret = cnwn_file_read_fixed(f, 4, buffer);
    if (ioret < 0) {
        cnwn_set_error("%s (erf version)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    for (int i = 0; i < ioret && i < 4; i++)
        ret_header->internal.version_str[i] = (buffer[i] > 32 ? buffer[i] : ' ');
    ret_header->version = cnwn_erf_version_parse(ret_header->internal.version_str);
    if (ret_header->version.major == 0 && ret_header->version.minor == 0) {
        char tmps[128];
        cnwn_strescape(tmps, sizeof(tmps), ret_header->internal.type_str, "\"", "\\");
        cnwn_set_error("invalid ERF version \"%s\"", tmps);
        return -1;
    }
    if (!cnwn_erf_version_support(&ret_header->version)) {
        char tmps[128];
        cnwn_strescape(tmps, sizeof(tmps), ret_header->internal.version_str, "\"", "\\");
        cnwn_set_error("unsupported ERF version \"%s\"", tmps);
        return -1;
    }
    // Num string entries
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (num string entries)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->num_string_entries = tmp32;
    if (ret_header->num_string_entries > CNWN_ERF_STRING_ENTRIES_MAX) {
        cnwn_set_error("too many string entries (%d<%d)", ret_header->num_string_entries, CNWN_ERF_STRING_ENTRIES_MAX);
        return -1;
    }
    // String entries size
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (string entries size)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->internal.string_entries_size = tmp32;
    // Num resource entries
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (num resource entries)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->num_resource_entries = tmp32;
    if (ret_header->num_resource_entries > CNWN_ERF_RESOURCE_ENTRIES_MAX) {
        cnwn_set_error("too many resource entries (%d<%d)", ret_header->num_resource_entries, CNWN_ERF_RESOURCE_ENTRIES_MAX);
        return -1;
    }
    // String entries offset
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (string entries offset)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->internal.string_entries_offset = tmp32;
    if (ret_header->num_string_entries > 0 && ret_header->internal.string_entries_offset < 160) {
        cnwn_set_error("invalid string entries offset (%"PRId64"<160)", ret_header->internal.string_entries_offset);
        return -1;
    }
    // Keys offset
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (keys offset)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->internal.keys_offset = tmp32;
    if (ret_header->num_resource_entries > 0 && ret_header->internal.keys_offset < 160) {
        cnwn_set_error("invalid keys offset (%"PRId64"<160)", ret_header->internal.keys_offset);
        return -1;
    }
    if (ret_header->num_resource_entries > 0 && ret_header->num_string_entries > 0
        && ret_header->internal.keys_offset < ret_header->internal.string_entries_offset + ret_header->internal.string_entries_size) {
        cnwn_set_error("invalid keys offset (%"PRId64"<%"PRId64")", ret_header->internal.keys_offset, ret_header->internal.string_entries_offset + ret_header->internal.string_entries_size);
        return -1;
    }
    // Values offset
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (values offset)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_header->internal.values_offset = tmp32;
    if (ret_header->num_resource_entries > 0 && ret_header->internal.values_offset < 160) {
        cnwn_set_error("invalid values offset (%"PRId64"<160)", ret_header->internal.values_offset);
        return -1;
    }
    // if (ret_header->num_resource_entries > 0 && ret_header->internal.values_offset < ret_header->internal.keys_offset + ret_header->num_resource_entries * (key_size + 8)) {
    //     cnwn_set_error("invalid values offset (%"PRId64"<%"PRId64")", ret_header->internal.values_offset, ret_header->internal.keys_offset + ret_header->num_resource_entries * (key_size + 8));
    //     return -1;
    // }
    return ret;
}

int64_t cnwn_erf_header_read_string_entry(cnwn_File * f, cnwn_ErfStringEntry * ret_entry)
{
    int64_t ioret;
    int64_t ret = 0;
    uint32_t tmp32;
    ioret = cnwn_file_get_seek(f);
    if (ioret < 0) {
        cnwn_set_error("%s (get seek)", cnwn_get_error());
        return -1;
    }
    ret_entry->internal.entry_offset = ioret;
    ret_entry->offset = ret_entry->internal.entry_offset + 8;
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (lang id)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->lang_id = tmp32;
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (size)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->size = tmp32;            
    if (ret_entry->size < 8) {
        cnwn_set_error("size too small (%"PRId64">=%d)", ret_entry->size, 8);
        return -1;
    }
    if (ret_entry->size > CNWN_ERF_STRING_ENTRY_MAX_SIZE) {
        cnwn_set_error("size too large (%"PRId64"<%d)", ret_entry->size, CNWN_ERF_STRING_ENTRY_MAX_SIZE);
        return -1;
    }
    ret += ret_entry->size;
    return ret;
}

int64_t cnwn_erf_header_read_resource_entry_key(cnwn_File * f, const cnwn_ErfVersion * version, cnwn_ErfResourceEntry * ret_entry)
{
    int key_size = (version != NULL && version->minor > 0 ? 32 : 16);
    uint8_t buffer[key_size];
    int64_t ioret;
    int64_t ret = 0;
    uint32_t tmp32;
    uint16_t tmp16;
    ioret = cnwn_file_get_seek(f);
    if (ioret < 0) {
        cnwn_set_error("%s (get seek)", cnwn_get_error());
        return -1;
    }
    ret_entry->internal.key_offset = ioret;
    ioret = cnwn_file_read_fixed(f, key_size, buffer);
    if (ioret < 0) {
        cnwn_set_error("%s (key)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->key = malloc(sizeof(char) * (ioret + 1));
    int soffset = 0;
    for (int j = 0; j < ioret; j++)
        if (buffer[j] > 32)
            ret_entry->key[soffset++] = buffer[j];
    ret_entry->key[soffset] = 0;            
    if (cnwn_strisblank(ret_entry->key)) {
        cnwn_set_error("empty key");
        return -1;
    }
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (ID)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->internal.id = tmp32;            
    ioret = cnwn_file_readu16(f, &tmp16);
    if (ioret < 0) {
        cnwn_set_error("%s (type)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->internal.type = tmp16;
    ret_entry->type = cnwn_resource_type_from_internal_type(ret_entry->internal.type);
    // if (!CNWN_RESOURCE_TYPE_VALID(ret_entry->type)) {
    //     cnwn_set_error("resource entry #%d has invalid type (%d)", i, ret_entry->internal.type);
    //     cnwn_erf_header_deinit(&header);
    //     return -1;
    // }
    ioret = cnwn_file_readu16(f, &tmp16);
    if (ioret < 0) {
        cnwn_set_error("%s (unused)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    char tmppath[CNWN_PATH_MAX_SIZE];
    const char * extension = CNWN_RESOURCE_TYPE_EXTENSION(ret_entry->type);
    if (!cnwn_strisblank(extension))
        snprintf(tmppath, sizeof(tmppath), "%s.%s", ret_entry->key, extension);
    else
        snprintf(tmppath, sizeof(tmppath), "%s", ret_entry->key);
    ret_entry->filename = cnwn_strdup(tmppath);
    ret_entry->internal.unused = tmp16;
    return ret;
}

int64_t cnwn_erf_header_read_resource_entry_value(cnwn_File * f, cnwn_ErfResourceEntry * ret_entry)
{
    int64_t ioret;
    int64_t ret = 0;
    uint32_t tmp32;
    ioret = cnwn_file_get_seek(f);
    if (ioret < 0) {
        cnwn_set_error("%s (get seek)", cnwn_get_error());
        return -1;
    }
    ret_entry->internal.value_offset = ioret;
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (resource offset)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->offset = tmp32;            
    ioret = cnwn_file_readu32(f, &tmp32);
    if (ioret < 0) {
        cnwn_set_error("%s (resource size)", cnwn_get_error());
        return -1;
    }
    ret += ioret;
    ret_entry->size = tmp32;
    return ret;
}

void cnwn_erf_header_free(cnwn_ErfHeader * header)
{
    if (header != NULL) {
        cnwn_erf_header_deinit(header);
        free(header);
    }
}


int64_t cnwn_erf_header_read(cnwn_File * f, cnwn_ErfHeader ** ret_header)
{
    int64_t ret = 0;
    int64_t ioret;
    cnwn_ErfHeader header = {0};
    ioret = cnwn_erf_header_read_top(f, &header);
    if (ioret < 0)
        return -1;
    ret += ioret;
    // String entries
    if (header.num_string_entries > 0) {
        ioret = cnwn_file_seek(f, header.internal.string_entries_offset);
        if (ioret < 0) {
            cnwn_set_error("%s (seek string entries offset)", cnwn_get_error());
            cnwn_erf_header_deinit(&header);
            return -1;
        }
        header.string_entries = malloc(sizeof(cnwn_ErfStringEntry) * header.num_string_entries);
        memset(header.string_entries, 0, sizeof(cnwn_ErfStringEntry) * header.num_string_entries);
        int64_t strings_size = 0;
        for (int i = 0; i < header.num_string_entries; i++) {
            int64_t rseret = cnwn_erf_header_read_string_entry(f, header.string_entries + i);
            if (rseret < 0) {
                cnwn_set_error("string entry #%d %s", i, cnwn_get_error());
                cnwn_erf_header_deinit(&header);
                return -1;
            }
            ret += rseret;
            strings_size += rseret;
            ioret = cnwn_file_seek_delta(f, header.string_entries[i].size);
            if (ioret < 0) {
                cnwn_set_error("string entry #%d %s (seeking next string entry)", i, cnwn_get_error());
                cnwn_erf_header_deinit(&header);
                return -1;
            }
        }
        if (strings_size < header.internal.string_entries_size) {
            cnwn_set_error("not enough string entries read for string entries size (%"PRId64", expected %"PRId64")", strings_size, header.internal.string_entries_size);
            cnwn_erf_header_deinit(&header);
            return -1;
        }
    }
    // Resource entries 
    if (header.num_resource_entries > 0) {
        ioret = cnwn_file_seek(f, header.internal.keys_offset);
        if (ioret < 0) {
            cnwn_set_error("%s (seek keys offset)", cnwn_get_error());
            cnwn_erf_header_deinit(&header);
            return -1;
        }
        header.resource_entries = malloc(sizeof(cnwn_ErfResourceEntry) * header.num_resource_entries);
        memset(header.resource_entries, 0, sizeof(cnwn_ErfResourceEntry) * header.num_resource_entries);
        // Keys
        for (int i = 0; i < header.num_resource_entries; i++) {
            int64_t rekret = cnwn_erf_header_read_resource_entry_key(f, &header.version, header.resource_entries + i);
            if (rekret < 0) {
                cnwn_set_error("resource entry key #%d %s", i, cnwn_get_error());
                cnwn_erf_header_deinit(&header);
                return -1;
            }
            ret += rekret;
        }
        ioret = cnwn_file_seek(f, header.internal.values_offset);
        if (ioret < 0) {
            cnwn_set_error("%s (seek values offset)", cnwn_get_error());
            cnwn_erf_header_deinit(&header);
            return -1;
        }
        // Values
        for (int i = 0; i < header.num_resource_entries; i++) {
            int64_t valret = cnwn_erf_header_read_resource_entry_value(f, header.resource_entries + i);
            if (valret < 0) {
                cnwn_set_error("resource entry value #%d %s", i, cnwn_get_error());
                cnwn_erf_header_deinit(&header);
                return -1;
            }
            ret += valret;
        }
    }
    if (ret_header != NULL) {
        *ret_header = malloc(sizeof(cnwn_ErfHeader));
        memcpy(*ret_header, &header, sizeof(cnwn_ErfHeader));
    } else {
        if (header.string_entries != NULL)
            free(header.string_entries);
        if (header.resource_entries != NULL)
            free(header.resource_entries);
    }
    return ret;
}


int64_t cnwn_erf_header_sum_resources(const cnwn_ErfHeader * header)
{
    int64_t total_size = 0;
    if (header != NULL) {
        for (int i = 0; i < header->num_resource_entries; i++)
            total_size += header->resource_entries[i].size;
    }
    return total_size;
}


