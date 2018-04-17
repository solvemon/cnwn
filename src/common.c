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
    if (len > 0)
        memcpy(CNWN_ERROR_MESSAGE, tmps, sizeof(char) * len);
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
