#include "cnwn.h"

const cnwn_ResourceInfo CNWN_RESOURCE_INFOS[CNWN_MAX_RESOURCE_TYPE] = {
    {"", "", -1},
    {"res", "",  0},
    {"bmp", "",  1},
    {"mve", "",  2},
    {"tga", "",  3},
    {"wav", "",  4},
    {"wfx", "",  5},
    {"plt", "",  6},
    {"ini", "",  7},
    {"mp3", "",  8},
    {"mpg", "",  9},
    {"txt", "",  10},
    {"plh", "",  2000},
    {"tex", "",  2001},
    {"mdl", "",  2002},
    {"thg", "",  2003},
    {"fnt", "",  2005},
    {"lua", "",  2007},
    {"slt", "",  2008},
    {"nss", "",  2009},
    {"ncs", "",  2010},
    {"mod", "",  2011},
    {"are", "",  2012},
    {"set", "",  2013},
    {"ifo", "",  2014},
    {"bic", "",  2015},
    {"wok", "",  2016},
    {"2da", "",  2017},
    {"tlk", "",  2018},
    {"txi", "",  2022},
    {"git", "",  2023},
    {"bti", "",  2024},
    {"uti", "",  2025},
    {"btc", "",  2026},
    {"utc", "",  2027},
    {"dlg", "",  2029},
    {"itp", "",  2030},
    {"btt", "",  2031},
    {"utt", "",  2032},
    {"dds", "",  2033},
    {"bts", "",  2034},
    {"uts", "",  2035},
    {"ltr", "",  2036},
    {"gff", "",  2037},
    {"fac", "",  2038},
    {"bte", "",  2039},
    {"ute", "",  2040},
    {"btd", "",  2041},
    {"utd", "",  2042},
    {"btp", "",  2043},
    {"utp", "",  2044},
    {"dft", "",  2045},
    {"gic", "",  2046},
    {"gui", "",  2047},
    {"css", "",  2048},
    {"ccs", "",  2049},
    {"btm", "",  2050},
    {"utm", "",  2051},
    {"dwk", "",  2052},
    {"pwk", "",  2053},
    {"btg", "",  2054},
    {"utg", "",  2055},
    {"jrl", "",  2056},
    {"sav", "",  2057},
    {"utw", "",  2058},
    {"4pc", "",  2059},
    {"ssf", "",  2060},
    {"hak", "",  2061},
    {"nwm", "",  2062},
    {"bik", "",  2063},
    {"ndb", "",  2064},
    {"ptm", "",  2065},
    {"ptt", "",  2066},
    {"bak", "",  2067},
    {"osc", "",  3000},
    {"usc", "",  3001},
    {"trn", "",  3002},
    {"utr", "",  3003},
    {"uen", "",  3004},
    {"ult", "",  3005},
    {"sef", "",  3006},
    {"pfx", "",  3007},
    {"cam", "",  3008},
    {"lfx", "",  3009},
    {"bfx", "",  3010},
    {"upe", "",  3011},
    {"ros", "",  3012},
    {"rst", "",  3013},
    {"ifx", "",  3014},
    {"pfb", "",  3015},
    {"zip", "",  3016},
    {"wmp", "",  3017},
    {"bbx", "",  3018},
    {"tfx", "",  3019},
    {"wlk", "",  3020},
    {"xml", "",  3021},
    {"scc", "",  3022},
    {"ptx", "",  3033},
    {"ltx", "",  3034},
    {"trx", "",  3035},
    {"mdb", "",  4000},
    {"mda", "",  4001},
    {"spt", "",  4002},
    {"gr2", "",  4003},
    {"fxa", "",  4004},
    {"fxe", "",  4005},
    {"jpg", "",  4007},
    {"pwc", "",  4008},
    {"ids", "",  9996},
    {"erf", "",  9997},
    {"bif", "",  9998},
    {"key", "",  9999}
};

cnwn_ResourceType cnwn_resource_type_from_erf_type(int erf_type)
{
    if (erf_type < 0 || erf_type > 9999)
        return CNWN_RESOURCE_TYPE_NONE;
    for (cnwn_ResourceType rt = CNWN_RESOURCE_TYPE_NONE + 1; rt < CNWN_MAX_RESOURCE_TYPE; rt++) 
        if (CNWN_RESOURCE_INFOS[rt].erf_type == erf_type)
            return rt;
    return CNWN_RESOURCE_TYPE_NONE;
}

cnwn_ResourceType cnwn_resource_type_from_extension(const char * extension)
{
    if (extension == NULL || extension[0] == 0)
        return CNWN_RESOURCE_TYPE_NONE;
    char usable_extension[5] = {0};
    int soffset = 0;
    for (int i = 0; i < 3 && extension[i] != 0; i++)
        if (extension[i] > 32)
            usable_extension[soffset++] = (extension[i] >= 'A' && extension[i] <= 'Z' ? extension[i] + 32 : extension[i]);
    for (cnwn_ResourceType rt = CNWN_RESOURCE_TYPE_NONE + 1; rt < CNWN_MAX_RESOURCE_TYPE; rt++) 
        if (strcmp(usable_extension, CNWN_RESOURCE_INFOS[rt].extension) == 0)
            return rt;
    return CNWN_RESOURCE_TYPE_NONE;
}
