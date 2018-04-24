#include "cnwn/resource_type.h"

const cnwn_ResourceDefinition CNWN_DEFINED_RESOURCES[CNWN_MAX_RESOURCE_TYPE] = {
    {"", -1, ""},
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
    {"mod", 2011, "nwn/mod"},
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
    {"key", 9999, ""}
};

cnwn_ResourceType cnwn_resource_type_from_internal_type(int internal_type)
{
    if (internal_type < 0 || internal_type > 9999)
        return CNWN_RESOURCE_TYPE_NONE;
    for (cnwn_ResourceType rt = CNWN_RESOURCE_TYPE_NONE + 1; rt < CNWN_MAX_RESOURCE_TYPE; rt++) 
        if (CNWN_DEFINED_RESOURCES[rt].internal_type == internal_type)
            return rt;
    return CNWN_RESOURCE_TYPE_NONE;
}

cnwn_ResourceType cnwn_resource_type_from_extension(const char * path)
{
    if (path == NULL || path[0] == 0)
        return CNWN_RESOURCE_TYPE_NONE;
    char tmps[CNWN_PATH_MAX_SIZE];
    int len = cnwn_path_extensionpart(tmps, sizeof(tmps), path);
    if (len <= 0)
        len = cnwn_path_basepart(tmps, sizeof(tmps), path);
    if (len > 0)
        for (cnwn_ResourceType rt = CNWN_RESOURCE_TYPE_NONE + 1; rt < CNWN_MAX_RESOURCE_TYPE; rt++) 
            if (cnwn_strcmpi(tmps, CNWN_DEFINED_RESOURCES[rt].extension) == 0) 
                return rt;
    return CNWN_RESOURCE_TYPE_NONE;
}
