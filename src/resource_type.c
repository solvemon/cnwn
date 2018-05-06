#include "cnwn/resource_type.h"

const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFOS[] = {
    {"res", 0, ""},
    {"bmp", 1, "bitmap image"},
    {"mve", 2, ""},
    {"tga", 3, "targa image"},
    {"wav", 4, "wave audio"},
    {"wfx", 5, ""},
    {"plt", 6, ""},
    {"ini", 7, "config"},
    {"mp3", 8, "mp3 audio"},
    {"mpg", 9, "mpeg video"},
    {"txt", 10, "text"},
    {"plh", 2000, ""},
    {"tex", 2001, "texture"},
    {"mdl", 2002, ""},
    {"thg", 2003, ""},
    {"fnt", 2005, "font"},
    {"lua", 2007, "lua script"},
    {"slt", 2008, ""},
    {"nss", 2009, "nwscript"},
    {"ncs", 2010, "compiled nwscript"},
    {"mod", 2011, ""},
    {"are", 2012, "area"},
    {"set", 2013, ""},
    {"ifo", 2014, ""},
    {"bic", 2015, ""},
    {"wok", 2016, ""},
    {"2da", 2017, ""},
    {"tlk", 2018, ""},
    {"txi", 2022, ""},
    {"git", 2023, ""},
    {"bti", 2024, ""},
    {"uti", 2025, ""},
    {"btc", 2026, ""},
    {"utc", 2027, ""},
    {"dlg", 2029, ""},
    {"itp", 2030, ""},
    {"btt", 2031, ""},
    {"utt", 2032, ""},
    {"dds", 2033, ""},
    {"bts", 2034, ""},
    {"uts", 2035, ""},
    {"ltr", 2036, ""},
    {"gff", 2037, ""},
    {"fac", 2038, ""},
    {"bte", 2039, ""},
    {"ute", 2040, ""},
    {"btd", 2041, ""},
    {"utd", 2042, ""},
    {"btp", 2043, ""},
    {"utp", 2044, ""},
    {"dft", 2045, ""},
    {"gic", 2046, ""},
    {"gui", 2047, ""},
    {"css", 2048, ""},
    {"ccs", 2049, ""},
    {"btm", 2050, ""},
    {"utm", 2051, ""},
    {"dwk", 2052, ""},
    {"pwk", 2053, ""},
    {"btg", 2054, ""},
    {"utg", 2055, ""},
    {"jrl", 2056, ""},
    {"sav", 2057, ""},
    {"utw", 2058, ""},
    {"4pc", 2059, ""},
    {"ssf", 2060, ""},
    {"hak", 2061, ""},
    {"nwm", 2062, ""},
    {"bik", 2063, ""},
    {"ndb", 2064, ""},
    {"ptm", 2065, ""},
    {"ptt", 2066, ""},
    {"bak", 2067, ""},
    {"osc", 3000, ""},
    {"usc", 3001, ""},
    {"trn", 3002, ""},
    {"utr", 3003, ""},
    {"uen", 3004, ""},
    {"ult", 3005, ""},
    {"sef", 3006, ""},
    {"pfx", 3007, ""},
    {"cam", 3008, ""},
    {"lfx", 3009, ""},
    {"bfx", 3010, ""},
    {"upe", 3011, ""},
    {"ros", 3012, ""},
    {"rst", 3013, ""},
    {"ifx", 3014, ""},
    {"pfb", 3015, ""},
    {"zip", 3016, ""},
    {"wmp", 3017, ""},
    {"bbx", 3018, ""},
    {"tfx", 3019, ""},
    {"wlk", 3020, ""},
    {"xml", 3021, ""},
    {"scc", 3022, ""},
    {"ptx", 3033, ""},
    {"ltx", 3034, ""},
    {"trx", 3035, ""},
    {"mdb", 4000, ""},
    {"mda", 4001, ""},
    {"spt", 4002, ""},
    {"gr2", 4003, ""},
    {"fxa", 4004, ""},
    {"fxe", 4005, ""},
    {"jpg", 4007, ""},
    {"pwc", 4008, ""},
    {"ids", 9996, ""},
    {"erf", 9997, "ERF"},
    {"bif", 9998, ""},
    {"key", 9999, ""},
    {"", -1, ""} // Sentinel
};

static cnwn_ResourceTypeInfo invalid_info = {"", -1, ""};

const cnwn_ResourceTypeInfo * cnwn_resource_type_info(cnwn_ResourceType resource_type)
{
    for (int i = 0; i < CNWN_MAX_RESOURCE_TYPE && CNWN_RESOURCE_TYPE_INFOS[i].type >= 0 && CNWN_RESOURCE_TYPE_INFOS[i].type < CNWN_MAX_RESOURCE_TYPE; i++) 
        if (CNWN_RESOURCE_TYPE_INFOS[i].type == resource_type)
            return CNWN_RESOURCE_TYPE_INFOS + i;
    return &invalid_info;
}


cnwn_ResourceType cnwn_resource_type_from_path(const char * path)
{
    char extension[CNWN_PATH_MAX_SIZE];
    cnwn_path_extensionpart(extension, sizeof(extension), path);
    cnwn_path_filenamepart(extension, sizeof(extension), extension);
    for (int i = 0; i < CNWN_MAX_RESOURCE_TYPE && CNWN_RESOURCE_TYPE_INFOS[i].type >= 0 && CNWN_RESOURCE_TYPE_INFOS[i].type < CNWN_MAX_RESOURCE_TYPE; i++) 
        if (cnwn_strcmpi(CNWN_RESOURCE_TYPE_INFOS[i].extension, extension) == 0)
            return i;
    return CNWN_RESOURCE_TYPE_INVALID;
}

void cnwn_localized_string_init(cnwn_LocalizedString * localized_string, int language_id, const char * text)
{
    memset(localized_string, 0, sizeof(cnwn_LocalizedString));
    localized_string->language_id = language_id;
    localized_string->text = cnwn_strdup(text != NULL ? text : "");
}

int64_t cnwn_localized_string_init_from_file(cnwn_LocalizedString * localized_string, cnwn_File * f)
{
    int64_t bytes = 0;
    int32_t language_id;
    int32_t size;
    int64_t ret;
    ret = cnwn_file_read32(f, &language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading language id");
        return -1;
    }
    bytes += ret;
    ret = cnwn_file_read32(f, &size);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "reading size");
        return -1;
    }
    if (size > 0) {
        char * text = malloc(sizeof(char) * (size + 1));
        ret = cnwn_file_read_string(f, size, text);        
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "reading text");
            free(text);
            return -1;
        }
        cnwn_localized_string_init(localized_string, language_id, text);
        free(text);
    } else
        cnwn_localized_string_init(localized_string, language_id, "");
    return ret;
}

void cnwn_localized_string_deinit(cnwn_LocalizedString * localized_string)
{
    if (localized_string != NULL && localized_string->text != NULL)
        free(localized_string->text);
}

int64_t cnwn_localized_string_write(const cnwn_LocalizedString * localized_string, cnwn_File * f)
{
    int64_t bytes = 0;
    int64_t ret;
    int textlen = cnwn_strlen(localized_string->text);
    ret = cnwn_file_write32(f, localized_string->language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "writing language id");
        return -1;
    }
    bytes += ret;
    ret = cnwn_file_write32(f, localized_string->language_id);
    if (ret < 0) {
        cnwn_set_error("%s (%s)", cnwn_get_error(), "writing size");
        return -1;
    }
    bytes += ret;
    if (textlen > 0) {
        ret = cnwn_file_write_string(f, localized_string->text);
        if (ret < 0) {
            cnwn_set_error("%s (%s)", cnwn_get_error(), "writing text");
            return -1;
        }
        bytes += ret;
    }
    return ret;
}
