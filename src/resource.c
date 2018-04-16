#include "cnwn/resource.h"

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
    char usable_extension[4] = {0};
    int soffset = 0;
    for (int i = 0; i < 3 && extension[i] != 0; i++)
        if (extension[i] > 32)
            usable_extension[soffset++] = (extension[i] >= 'A' && extension[i] <= 'Z' ? extension[i] + 32 : extension[i]);
    for (cnwn_ResourceType rt = CNWN_RESOURCE_TYPE_NONE + 1; rt < CNWN_MAX_RESOURCE_TYPE; rt++) {
        if (strcmp(usable_extension, CNWN_RESOURCE_INFOS[rt].extension) == 0) 
            return rt;
    }
    return CNWN_RESOURCE_TYPE_NONE;
}


int cnwn_resource_to_filename(cnwn_ResourceType resource_type, const char * key, int max_size, char * ret_filename)
{
    if (key == NULL || key[0] == 0 || (max_size <= 0 && ret_filename != NULL)) {
        if (ret_filename != NULL && max_size > 0)
            ret_filename[0] = 0;
        return 0;
    }
    char tmps[CNWN_PATH_MAX_SIZE];
    const char * ext = CNWN_RESOURCE_TYPE_EXTENSION(resource_type);
    int slen = 0;
    for (int i = 0; key[i] != 0 && slen < sizeof(tmps) - 1 && i < 32; i++)
        if (key[i] > 32)
            tmps[slen++] = key[i];
    if (ext != NULL && ext[0] != 0 && slen < sizeof(tmps) - 1) {
        tmps[slen++] = '.';
        for (int i = 0; ext[i] != 0 && slen < sizeof(tmps) - 1 && i < 3; i++)
            if (ext[i] > 32)
                tmps[slen++] = ext[i];
    }
    tmps[slen] = 0;
    if (max_size > 0) {
        slen = CNWN_MIN(slen, max_size - 1);
        if (ret_filename != NULL) {
            if (slen > 0)
                memcpy(ret_filename, tmps, sizeof(char) * slen);
            ret_filename[slen] = 0;
        }
    }
    return slen;
}

int cnwn_resource_from_filename(const char * filename, cnwn_ResourceType * ret_resource_type, int max_size, char * ret_key)
{
    if (filename == NULL || filename[0] == 0) {
        if (max_size > 0 && ret_key != NULL)
            ret_key[0] = 0;
        if (ret_resource_type != NULL)
            *ret_resource_type = CNWN_RESOURCE_TYPE_NONE;
        return 0;
    }
    int flen = strlen(filename);
    char ext[4] = {0};
    char base[CNWN_PATH_MAX_SIZE];
    int offset = flen - 1;
    while (offset >= 0 && filename[offset] != '.')
        offset--;
    int extlen = flen - offset - 1;
    if (extlen > 0 && extlen <= 3)
        memcpy(ext, filename + offset + 1, sizeof(char) * extlen);
    for (int i = 0; ext[i] != 0 && i < 3; i++)
        if (ext[i] >= 'A' && ext[i] <= 'Z')
            ext[i] += 32;
    int offset2 = offset - 1;
    while (offset2 >= 0 && filename[offset2] != CNWN_PATH_SEPARATOR)
        offset2--;    
    int baselen = offset - offset2 - 1;
    baselen = CNWN_MIN(baselen, sizeof(base) - 1);
    int retlen = 0;
    for (int i = 0; i < baselen; i++)
        if (filename[offset2 + 1 + i] > 32)
            base[retlen++] = filename[offset2 + 1 + i];
    if (ret_resource_type != NULL)
        *ret_resource_type = cnwn_resource_type_from_extension(ext);
    retlen = CNWN_MIN(retlen, 32);
    if (max_size > 0) {
        retlen = CNWN_MIN(retlen, max_size - 1);
        if (ret_key != NULL) {
            if (retlen > 0) 
                memcpy(ret_key, base, sizeof(char) * retlen);
            ret_key[retlen] = 0;
        }
    }
    return retlen;
}


int cnwn_resource_key_clean(const char * s, int max_size, char * ret_s)
{
    int offset = 0;
    if (s != NULL) {
        for (int i = 0; s[i] != 0 && (offset < max_size - 1 || (max_size <= 0 && ret_s == NULL)); i++) {
            if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9') || s[i] == '_') {
                if (max_size > 0 && ret_s != NULL)
                    ret_s[offset++] = s[i];
                else
                    offset++;
            }
        }
    }
    if (ret_s != NULL)
        ret_s[offset] = 0;
    return offset;
}
