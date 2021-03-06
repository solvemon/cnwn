#include "cnwn/resource_type.h"

const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFOS[CNWN_MAX_RESOURCE_TYPE] = {
    {0, "res", ""},
    {1, "bmp", "bitmap image"},
    {2, "mve", "infinity engine movie"},
    {3, "tga", "targa image"},
    {4, "wav", "wave audio"},
    {5, "wfx", ""},
    {6, "plt", ""},
    {7, "ini", "config"},
    {8, "mp3", "mp3 audio"},
    {9, "mpg", "mpeg video"},
    {10, "txt", "text"},
    [11 ... 1999] = {-1, NULL, NULL},
    {2000, "plh", ""},
    {2001, "tex", "texture"},
    {2002, "mdl", ""},
    {2003, "thg", ""},
    {-1, NULL, NULL},
    {2005, "fnt", "font"},
    {-1, NULL, NULL},
    {2007, "lua", "lua script"},
    {2008, "slt", ""},
    {2009, "nss", "nwscript source"},
    {2010, "ncs", "nwscript bytecode"},
    {2011, "mod", "module"},
    {2012, "are", "area static"},
    {2013, "set", ""},
    {2014, "ifo", "module component"},
    {2015, "bic", "character"},
    {2016, "wok", "asian cooking utensil"},
    {2017, "2da", "text based data"},
    {2018, "tlk", "talk table"},
    [2019 ... 2021] = {-1, NULL, NULL},
    {2022, "txi", ""},
    {2023, "git", "area dynamic"},
    {2024, "bti", ""},
    {2025, "uti", "item"},
    {2026, "btc", ""},
    {2027, "utc", "creature"},
    {-1, NULL, NULL},
    {2029, "dlg", "conversation"},
    {2030, "itp", ""},
    {2031, "btt", ""},
    {2032, "utt", "trigger"},
    {2033, "dds", "direct draw surface"},
    {2034, "bts", ""},
    {2035, "uts", "placable sound"},
    {2036, "ltr", ""},
    {2037, "gff", "generic"},
    {2038, "fac", "factions"},
    {2039, "bte", ""},
    {2040, "ute", "encounter"},
    {2041, "btd", ""},
    {2042, "utd", "door"},
    {2043, "btp", ""},
    {2044, "utp", "placable"},
    {2045, "dft", ""},
    {2046, "gic", "area comments"},
    {2047, "gui", ""},
    {2048, "css", ""},
    {2049, "ccs", ""},
    {2050, "btm", ""},
    {2051, "utm", "store"},
    {2052, "dwk", ""},
    {2053, "pwk", ""},
    {2054, "btg", ""},
    {2055, "utg", ""},
    {2056, "jrl", "journal"},
    {2057, "sav", "save game"},
    {2058, "utw", "waypoint"},
    {2059, "4pc", ""},
    {2060, "ssf", ""},
    {2061, "hak", "hak (erf) container"},
    {2062, "nwm", "bioware module"},
    {2063, "bik", "bink video"},
    {2064, "ndb", "compiled script override"},
    {2065, "ptm", ""},
    {2066, "ptt", ""},
    {2067, "bak", ""},
    [2068 ... 2999] = {-1, NULL, NULL},
    {3000, "osc", ""},
    {3001, "usc", ""},
    {3002, "trn", "uncompiled terrain"},
    {3003, "utr", "trees"},
    {3004, "uen", ""},
    {3005, "ult", "light"},
    {3006, "sef", "special effect"},
    {3007, "pfx", "particle effect"},
    {3008, "cam", "campaign"},
    {3009, "lfx", "line effect"},
    {3010, "bfx", ""},
    {3011, "upe", ""},
    {3012, "ros", ""},
    {3013, "rst", ""},
    {3014, "ifx", ""},
    {3015, "pfb", ""},
    {3016, "zip", "zip compressed data"},
    {3017, "wmp", "world map"},
    {3018, "bbx", "billboard effect"},
    {3019, "tfx", ""},
    {3020, "wlk", ""},
    {3021, "xml", "user interface"},
    {3022, "scc", ""},
    {3033, "ptx", ""},
    {3034, "ltx", ""},
    {3035, "trx", "compiled terrain"},
    [3036 ... 3999] = {-1, NULL, NULL},
    {4000, "mdb", ""},
    {4001, "mda", ""},
    {4002, "spt", ""},
    {4003, "gr2", "animation skeleton"},
    {4004, "fxa", ""},
    {4005, "fxe", "lip sync"},
    {4007, "jpg", "jpeg image"},
    {4008, "pwc", "persistant world"},
    [4009 ... 9995] = {-1, NULL, NULL},
    {9996, "ids", ""},
    {9997, "erf", "erf container"},
    {9998, "bif", "bif container"},
    {9999, "key", ""}
};

const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFO_INVALID = {-1, NULL, NULL};

cnwn_ResourceType cnwn_resource_type_from_path(const char * path)
{
    char extension[CNWN_PATH_MAX_SIZE];
    if (cnwn_path_extensionpart(extension, sizeof(extension), path) == 0)
        cnwn_path_filenamepart(extension, sizeof(extension), path);
    else
        cnwn_path_filenamepart(extension, sizeof(extension), extension);
    cnwn_strstrip(extension, sizeof(extension), extension);
    cnwn_strlower(extension, sizeof(extension), extension);
    for (int i = 0; i < CNWN_MAX_RESOURCE_TYPE; i++) {
        if (!cnwn_strisblank(CNWN_RESOURCE_TYPE_INFOS[i].extension)
            && cnwn_strcmp(CNWN_RESOURCE_TYPE_INFOS[i].extension, extension) == 0)
            return i;
    }
    return CNWN_RESOURCE_TYPE_INVALID;
}

cnwn_ResourceTypeInfo cnwn_resource_type_info_from_path(const char * path)
{
    cnwn_ResourceType t = cnwn_resource_type_from_path(path);
    if (CNWN_RESOURCE_TYPE_VALID(t))
        return CNWN_RESOURCE_TYPE_INFOS[t];
    return CNWN_RESOURCE_TYPE_INFO_INVALID;
}
