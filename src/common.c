#include "cnwn/common.h"

static char CNWN_ERROR_MESSAGE[8192] = {0};

const char * cnwn_get_error(void)
{
    return CNWN_ERROR_MESSAGE;
}

void cnwn_set_error_va(const char * format, va_list args)
{
    char tmps[8192];
    int len = vsnprintf(tmps, sizeof(tmps), format, args);
    for (int i = 0; i < len; i++)
        if (tmps[i] >= 32)
            CNWN_ERROR_MESSAGE[i] = tmps[i];
    CNWN_ERROR_MESSAGE[len] = 0;
}

void cnwn_set_error(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    cnwn_set_error_va(format, args);
    va_end(args);
}


uint32_t cnwn_swap32(uint32_t i)
{
    #ifdef BIG_ENDIAN
    return ((((uint32_t)(i) & 0xff000000) >> 8) | (((uint32_t)(i) & 0x00ff0000) << 8) | (((uint32_t)(i) & 0x0000ff00) >> 8) | (((uint32_t)(i) & 0x000000ff) << 8));
    #else
    return i;
    #endif
}

uint16_t cnwn_swap16(uint16_t i)
{
    #ifdef BIG_ENDIAN
    return ((((uint16_t)(i) & 0xff00) >> 8) | (((uint16_t)(i) & 0x00ff) << 8))
    #else
    return i;
    #endif
}

void cnwn_free_strings(char ** strings)
{
    if (strings != NULL) {
        for (char ** s = strings; *s != NULL; s++) 
            free(*s);
        free(strings);
    }
}

int cnwn_count_strings(char ** strings)
{
    int ret = 0;
    if (strings != NULL) 
        for (char ** s = strings; *s != NULL; s++) 
            ret++;
    return ret;
}

char ** cnwn_append_strings(char ** strings, char ** other)
{
    int count1 = cnwn_count_strings(strings);
    int count2 = cnwn_count_strings(other);
    char ** ret = malloc(sizeof(char *) * (count1 + count2 + 1));
    for (int i = 0; i < count1; i++)
        ret[i] = cnwn_string_dup(strings[i]);
    for (int i = 0; i < count2; i++)
        ret[i + count1] = cnwn_string_dup(strings[i]);
    ret[count1 + count2] = NULL;
    free(strings);
    return ret;
}

int cnwn_copy_string(char * s, int max_size, const char * ss, int len)
{
    int sslen = ss != NULL ? strlen(ss) : 0;
    if (len < 0)
        len = sslen;
    if (len > sslen)
        len = sslen;
    if (max_size > 0) {
        len = CNWN_MIN(len, max_size - 1);
        if (s != NULL) {
            if (len > 0)
                memmove(s, ss, sizeof(char) * len);
            s[len] = 0;
        }
    }
    return len;
}

bool cnwn_string_isempty(const char * s)
{
    if (s != NULL)
        for (int i = 0; s[i] != 0; i++)
            if (s[i] > 32)
                return false;
    return true;
}

char * cnwn_string_dup(const char * s)
{
    if (s != NULL) {
        int len = strlen(s);
        char * ret = malloc(sizeof(char) * (len + 1));
        if (len > 0)
            memcpy(ret, s, sizeof(char) * len);
        ret[len] = 0;
        return ret;
    }
    return NULL;
}

int cnwn_string_cmp(const char * s, const char * o)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    int offset = 0;
    while (s[offset] != 0 && o[offset] != 0 && s[offset] == o[offset])
        offset++;
    if ((unsigned char)s[offset] < (unsigned char)o[offset])
        return -1;
    if ((unsigned char)s[offset] > (unsigned char)o[offset])
        return 1;
    return 0;
}

int cnwn_string_ncmp(const char * s, const char * o, int n)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    int offset = 0;
    while (s[offset] != 0 && o[offset] != 0 && s[offset] == o[offset] && (n <= 0 || offset < n)) 
        offset++;
    if (offset < n) {
        if ((unsigned char)s[offset] < (unsigned char)o[offset])
            return -1;
        if ((unsigned char)s[offset] > (unsigned char)o[offset])
            return 1;
    }
    return 0;
}

int cnwn_string_cmpicase(const char * s, const char * o)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    int offset = 0;
    while (s[offset] != 0 && o[offset] != 0
           && (s[offset] >= 'A' && s[offset] <= 'Z' ? s[offset] + 32 : s[offset]) == (o[offset] >= 'A' && o[offset] <= 'Z' ? o[offset] + 32 : o[offset]))
        offset++;
    if ((unsigned char)s[offset] < (unsigned char)o[offset])
        return -1;
    if ((unsigned char)s[offset] > (unsigned char)o[offset])
        return 1;
    return 0;
}

int cnwn_string_ncmpicase(const char * s, const char * o, int n)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    int offset = 0;
    while (s[offset] != 0 && o[offset] != 0
           && (s[offset] >= 'A' && s[offset] <= 'Z' ? s[offset] + 32 : s[offset]) == (o[offset] >= 'A' && o[offset] <= 'Z' ? o[offset] + 32 : o[offset])
           && (n <= 0 || offset < n)) 
        offset++;
    if (offset < n) {
        if ((unsigned char)s[offset] < (unsigned char)o[offset])
            return -1;
        if ((unsigned char)s[offset] > (unsigned char)o[offset])
            return 1;
    }
    return 0;
}
