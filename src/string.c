#include "cnwn/string.h"

static bool cnwn_str_detect_esc(const char * s, int index, const char * esc, int esclen)
{
    bool escaped = false;
    while (index >= esclen) {
        index -= esclen;
        if (cnwn_strncmp(s + index, esc, esclen) != 0)
            break;
        escaped = !escaped;
    }
    return escaped;
}

static bool cnwn_str_detect_esci(const char * s, int index, const char * esc, int esclen)
{
    bool escaped = false;
    while (index >= esclen) {
        index -= esclen;
        if (cnwn_strncmpi(s + index, esc, esclen) != 0)
            break;
        escaped = !escaped;
    }
    return escaped;
}

int cnwn_strlen(const char * s)
{
    int len = 0;
    if (s != NULL)
        while (s[len] != 0)
            len++;
    return len;
}

int cnwn_strnlen(const char * s, int n)
{
    int len = 0;
    if (s != NULL && n > 0)
        while (s[len] != 0 && len < n)
            len++;
    return len;
}

int cnwn_strcmp(const char * s, const char * o)
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

int cnwn_strncmp(const char * s, const char * o, int n)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    if (n > 0) {
        int offset = 0;
        while (s[offset] != 0 && o[offset] != 0 && s[offset] == o[offset] && offset < n) 
            offset++;
        if (offset < n) {
            if ((unsigned char)s[offset] < (unsigned char)o[offset])
                return -1;
            if ((unsigned char)s[offset] > (unsigned char)o[offset])
                return 1;
        }
    }
    return 0;
}

int cnwn_strcmpi(const char * s, const char * o)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    int offset = 0;
    while (s[offset] != 0 && o[offset] != 0 && CNWN_CHAR_ICASE_EQ(s[offset], o[offset]))
        offset++;
    if ((unsigned char)s[offset] < (unsigned char)o[offset])
        return -1;
    if ((unsigned char)s[offset] > (unsigned char)o[offset])
        return 1;
    return 0;
}

int cnwn_strncmpi(const char * s, const char * o, int n)
{
    if (s == o)
        return 0;
    if (s == NULL && o != NULL)
        return -1;
    if (o == NULL && s != NULL)
        return 1;
    if (n > 0) {
        int offset = 0;
        while (s[offset] != 0 && o[offset] != 0 && CNWN_CHAR_ICASE_EQ(s[offset], o[offset]) && offset < n) 
            offset++;
        if (offset < n) {
            if ((unsigned char)s[offset] < (unsigned char)o[offset])
                return -1;
            if ((unsigned char)s[offset] > (unsigned char)o[offset])
                return 1;
        }
    }
    return 0;
}

char * cnwn_strdup(const char * s)
{
    if (s != NULL) {
        int len = cnwn_strlen(s);
        char * ret = malloc(sizeof(char) * (len + 1));
        if (len > 0)
            memcpy(ret, s, sizeof(char) * len);
        ret[len] = 0;
        return ret;
    }
    return NULL;
}

char * cnwn_strndup(const char * s, int n)
{
    if (s != NULL) {
        int len = cnwn_strnlen(s, n);
        char * ret = malloc(sizeof(char) * (len + 1));
        if (len > 0)
            memcpy(ret, s, sizeof(char) * len);
        ret[len] = 0;
        return ret;
    }
    return NULL;
}

char * cnwn_strndup2(const char * s, int i, int n)
{
    int len = cnwn_strlen(s);
    if (i < len)
        i += len;
    char * ret;
    if (i >= 0 && i < len) {
        if (i + n > len)
            n = len - i;
        ret = malloc(sizeof(char) * (n + 1));
        if (n > 0)
            memcpy(ret, s + i, sizeof(char) * n);
        ret[n] = 0;
    } else {
        ret = malloc(sizeof(char));
        ret[0] = 0;
    }
    return ret;
}

char * cnwn_strformat_va(const char * format, va_list args)
{
    va_list argscopy;
    va_copy(argscopy, args);
    int len = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    char * ret = malloc(sizeof(char) * (len + 1));
    vsnprintf(ret, len + 1, format, args);
    va_end(args);
    return ret;
}

char * cnwn_strformat(const char * format, ...)
{
    va_list args;
    va_start(args, format);
    char * ret = cnwn_strformat_va(format, args);
    va_end(args);
    return ret;
}

int cnwn_strcpy(char * r, int max_size, const char * s, int len)
{
    int slen = cnwn_strlen(s);
    if (len < 0 || len > slen)
        len = slen;
    if (max_size > 0) {
        len = CNWN_MIN(len, max_size - 1);
        if (r != NULL) 
            memmove(r, s, sizeof(char) * len);
    }
    if (r != NULL && max_size > 0)
        r[len] = 0;
    return len;
}

int cnwn_strupper(char * r, int max_size, const char * s)
{
    int len = cnwn_strlen(s);
    if (max_size > 0) {
        if (len > max_size - 1)
            len = max_size - 1;
        if (r != NULL) {
            for (int i = 0; i < len; i++)
                r[i] = (s[i] >= 'a' && s[i] <= 'z' ? s[i] - 32 : s[i]);
            r[len] = 0;
        }
    }
    return len;
}

int cnwn_strlower(char * r, int max_size, const char * s)
{
    int len = cnwn_strlen(s);
    if (max_size > 0) {
        if (len > max_size - 1)
            len = max_size - 1;
        if (r != NULL) {
            for (int i = 0; i < len; i++)
                r[i] = (s[i] >= 'A' && s[i] <= 'Z' ? s[i] + 32 : s[i]);
            r[len] = 0;
        }
    }
    return len;
}

int cnwn_strcat_va(char * r, int max_size, const char * sep, va_list args)
{
    const char * s;
    int seplen = cnwn_strlen(sep);
    int offset = 0;
    int slen = -1;
    while ((s = va_arg(args, char *)) != NULL && offset < max_size - 1) {
        if (slen >= 0 && seplen > 0) {
            int copylen = CNWN_MINMAX(seplen, 0, max_size - offset - 1);
            if (r != NULL) {
                if (copylen > 0)
                    memcpy(r + offset, sep, sizeof(char) * copylen);
                offset += copylen;
            } else
                offset += copylen;
        }
        slen = cnwn_strlen(s);
        if (max_size > 0) {
            int copylen = CNWN_MINMAX(slen, 0, max_size - offset - 1);
            if (r != NULL) {
                if (copylen > 0)
                    memcpy(r + offset, s, sizeof(char) * copylen);
                offset += copylen;
            } else
                offset += copylen;
        } else 
            offset += slen;
    }
    if (r != NULL && max_size > 0)
        r[offset] = 0;
    return offset;
}

int cnwn_strcat(char * r, int max_size, const char * sep, ...)
{
    va_list args;
    va_start(args, sep);
    int ret = cnwn_strcat_va(r, max_size, sep, args);
    va_end(args);
    return ret;
}

int cnwn_strcat2(char * r, int max_size, const char * sep, int num_strings, const char ** strings)
{
    int offset = 0;
    if (strings != NULL) {
        for (int i = 0; i < num_strings && (offset < max_size - 1 || max_size <= 0); i++) {
            int slen = cnwn_strlen(strings[i]);
            if (max_size > 0) {
                slen = CNWN_MINMAX(slen, 0, max_size - offset - 1);
                if (r != NULL && slen > 0) 
                    memcpy(r + offset, strings[i], sizeof(char) * slen);
            }
            offset += slen;
        }
    }
    if (max_size > 0 && r != NULL)
        r[offset] = 0;
    return offset;    
}


bool cnwn_strisblank(const char * s)
{
    if (s != NULL)
        for (int i = 0; s[i] != 0; i++)
            if (s[i] > 32)
                return false;
    return true;
}

int cnwn_strstripstart(char * r, int max_size, const char * s)
{
    if (s != NULL)
        for (int i = 0; s[i] != 0; i++)
            if (s[i] > 32) 
                return cnwn_strcpy(r, max_size, s  + i, -1);
    return cnwn_strcpy(r, max_size, s, -1);
}

int cnwn_strstripend(char * r, int max_size, const char * s)
{
    if (s != NULL) {
        int slen = cnwn_strlen(s);
        for (int i = slen - 1; i >= 0; i--)
            if (s[i] > 32) 
                return cnwn_strcpy(r, max_size, s, i);
    }
    return cnwn_strcpy(r, max_size, s, -1);
}

int cnwn_strstrip(char * r, int max_size, const char * s)
{
    int slen = cnwn_strlen(s);
    if (slen > 0) {
        int start = 0;
        int end = slen - 1;
        for (int i = 0; s[i] != 0; i++) 
            if (s[i] > 32) {
                start = i;
                break;
            }
        for (int i = slen - 1; i >= 0; i--)
            if (s[i] > 32) {
                end = i;
                break;
            }
        return cnwn_strcpy(r, max_size, s + start, end - start + 1);
    }
    return cnwn_strcpy(r, max_size, s, -1);
}

bool cnwn_strstartswith(const char * s, const char * sub)
{
    int sublen = cnwn_strlen(sub);
    return (sublen > 0 && cnwn_strncmp(s, sub, sublen) == 0);
}

bool cnwn_strendswith(const char * s, const char * sub)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    return (slen > 0 && sublen > 0 && cnwn_strncmp(s + slen - sublen, sub, sublen) == 0);
}

bool cnwn_strstartswithi(const char * s, const char * sub)
{
    int sublen = cnwn_strlen(sub);
    return (sublen > 0 && cnwn_strncmpi(s, sub, sublen) == 0);
}

bool cnwn_strendswithi(const char * s, const char * sub)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    return (slen > 0 && sublen > 0 && cnwn_strncmpi(s + slen - sublen, sub, sublen) == 0);
}

int cnwn_strfind(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index <= slen - sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i < slen; i++) 
            if (cnwn_strncmp(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esc(s, i, esc, esclen)))
                return i;
    }
    return -1;
}

int cnwn_strfindi(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index <= slen - sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i < slen; i++) 
            if (cnwn_strncmpi(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esci(s, i, esc, esclen)))
                return i;
    }
    return -1;
}

int cnwn_strrfind(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen - sublen + 1;
    if (index < 0)
        index = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index >= sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i >= 0; i--) 
            if (cnwn_strncmp(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esc(s, i, esc, esclen)))
                return i;
    }
    return -1;
}

int cnwn_strrfindi(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen - sublen + 1;
    if (index < 0)
        index = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index >= sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i >= 0; i--) 
            if (cnwn_strncmpi(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esci(s, i, esc, esclen)))
                return i;
    }
    return -1;
}

int cnwn_strfindchar(const char * s, int index, char c, char e)
{
    if (s != NULL)
        for (int i = 0; s[i] != 0; i++)
            if (s[i] == c) {
                bool escaped = false;
                if (e != 0)
                    for (int j = i -1; j >= 0 && s[j] == e; j--)
                        escaped = !escaped;
                if (!escaped)
                    return i;
            }
    return -1;
}

int cnwn_strfindchari(const char * s, int index, char c, char e)
{
    if (s != NULL)
        for (int i = 0; s[i] != 0; i++)
            if (CNWN_CHAR_ICASE_EQ(s[i], c)) {
                bool escaped = false;
                if (e != 0)
                    for (int j = i -1; j >= 0 && CNWN_CHAR_ICASE_EQ(s[j], e); j--)
                        escaped = !escaped;
                if (!escaped)
                    return i;
            }
    return -1;
}

int cnwn_strcount(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    int count = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index < slen - sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i < slen; i++) 
            if (cnwn_strncmp(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esc(s, i, esc, esclen))) {
                count++;
                i += sublen - 1;
            }
    }
    return count;
}

int cnwn_strcounti(const char * s, int index, const char * sub, const char * esc)
{
    int slen = cnwn_strlen(s);
    int sublen = cnwn_strlen(sub);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    int count = 0;
    if (slen > 0 && sublen > 0 && sublen < slen && index < slen - sublen) {
        int esclen = cnwn_strlen(esc);
        for (int i = index; i < slen; i++) 
            if (cnwn_strncmpi(s + i, sub, sublen) == 0
                && (esclen <= 0 || !cnwn_str_detect_esci(s, i, esc, esclen))) {
                count++;
                i += sublen - 1;
            }
    }
    return count;
}

int cnwn_strescape(char * r, int max_size, const char * s, const char * sub, const char * esc)
{
    // FIXME: implement
    return cnwn_strcpy(r, max_size, s, -1);
}

int cnwn_strunescape(char * r, int max_size, const char * s, const char * sub, const char * esc)
{
    // FIXME: implement
    return cnwn_strcpy(r, max_size, s, -1);
}

int cnwn_strnoctl(char * r, int max_size, const char * s)
{
    int soffset = 0;
    if (s != NULL) {
        for (int i = 0; s[i] != 0; i++)
            if (r != NULL && soffset < max_size - 1)
                r[soffset++] = s[i] > 32 ? s[i] : 32;
            else
                soffset++;
    }
    if (r != NULL && max_size > 0)
        r[soffset] = 0;
    return soffset;
}


int cnwn_strinsert_va(char * r, int max_size, const char * s, int index, const char * format, va_list args)
{
    int slen = cnwn_strlen(s);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    if (index > slen)
        index = slen;
    va_list argscopy;
    va_copy(argscopy, args);
    int flen = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    int retlen = slen + flen;
    if (max_size > 0) {
        retlen = CNWN_MIN(retlen, max_size - 1);
        if (r != NULL) {
            char * tmpformat = malloc(sizeof(char) * (flen + 1));
            vsnprintf(tmpformat, flen + 1, format, args);
            char * tmp = malloc(sizeof(char) * (retlen + 1));
            int copylen = CNWN_MINMAX(index, 0, retlen);
            if (copylen > 0)
                memcpy(tmp, s, sizeof(char) * copylen);
            int copylen2 = CNWN_MINMAX(flen, 0, retlen - copylen);
            if (copylen2 > 0)
                memcpy(tmp + copylen, tmpformat, sizeof(char) * copylen2);
            int copylen3 = CNWN_MINMAX(slen - index, 0, retlen - copylen - copylen2);
            if (copylen3 > 0)
                memcpy(tmp + copylen + copylen2, s + index, sizeof(char) * copylen3);
            free(tmpformat);
            memcpy(r, tmp, sizeof(char) * retlen);
            r[retlen] = 0;
            free(tmp);
        }
    }
    return retlen;
}

int cnwn_strinsert(char * r, int max_size, const char * s, int index, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_strinsert_va(r, max_size, s, index, format, args);
    va_end(args);
    return ret;
}

char * cnwn_strinsert_realloc_va(char * s, int index, const char * format, va_list args)
{
    int slen = cnwn_strlen(s);
    if (index < 0)
        index += slen;
    if (index < 0)
        index = 0;
    if (index > slen)
        index = slen;
    va_list argscopy;
    va_copy(argscopy, args);
    int flen = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    int retlen = slen + flen;
    char * tmpformat = malloc(sizeof(char) * (flen + 1));
    vsnprintf(tmpformat, flen + 1, format, args);
    char * tmp = malloc(sizeof(char) * (retlen + 1));
    int copylen = CNWN_MINMAX(index, 0, retlen);
    if (copylen > 0)
        memcpy(tmp, s, sizeof(char) * copylen);
    int copylen2 = CNWN_MINMAX(flen, 0, retlen - copylen);
    if (copylen2 > 0)
        memcpy(tmp + copylen, tmpformat, sizeof(char) * copylen2);
    int copylen3 = CNWN_MINMAX(slen - index, 0, retlen - copylen - copylen2);
    if (copylen3 > 0)
        memcpy(tmp + copylen + copylen2, s + index, sizeof(char) * copylen3);
    free(tmpformat);
    tmp[retlen] = 0;
    if (s != NULL)
        free(s);
    return tmp;
}

char * cnwn_strinsert_realloc(char * s, int index, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    char * ret = cnwn_strinsert_realloc_va(s, index, format, args);
    va_end(args);
    return ret;
}

int cnwn_strappend_va(char * r, int max_size, const char * s, const char * format, va_list args)
{
    int slen = cnwn_strlen(s);
    va_list argscopy;
    va_copy(argscopy, args);
    int flen = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    int retlen = slen + flen;
    if (max_size > 0) {
        retlen = CNWN_MIN(retlen, max_size - 1);
        if (r != NULL) {
            char * tmpformat = malloc(sizeof(char) * (flen + 1));
            vsnprintf(tmpformat, flen + 1, format, args);
            char * tmp = malloc(sizeof(char) * (retlen + 1));
            int copylen = CNWN_MINMAX(slen, 0, retlen);
            if (copylen > 0)
                memcpy(tmp, s, sizeof(char) * copylen);
            int copylen2 = CNWN_MINMAX(flen, 0, retlen - copylen);
            if (copylen2 > 0)
                memcpy(tmp + copylen, tmpformat, sizeof(char) * copylen2);
            free(tmpformat);
            memcpy(r, tmp, sizeof(char) * retlen);
            r[retlen] = 0;
            free(tmp);
        }
    }
    return retlen;
}

int cnwn_strappend(char * r, int max_size, const char * s, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    int ret = cnwn_strappend_va(r, max_size, s, format, args);
    va_end(args);
    return ret;
}

char * cnwn_strappend_realloc_va(char * s, const char * format, va_list args)
{
    int slen = cnwn_strlen(s);
    va_list argscopy;
    va_copy(argscopy, args);
    int flen = vsnprintf(NULL, 0, format, argscopy);
    va_end(argscopy);
    int retlen = slen + flen;
    char * tmpformat = malloc(sizeof(char) * (flen + 1));
    vsnprintf(tmpformat, flen + 1, format, args);
    char * tmp = malloc(sizeof(char) * (retlen + 1));
    int copylen = CNWN_MINMAX(slen, 0, retlen);
    if (copylen > 0)
        memcpy(tmp, s, sizeof(char) * copylen);
    int copylen2 = CNWN_MINMAX(flen, 0, retlen - copylen);
    if (copylen2 > 0)
        memcpy(tmp + copylen, tmpformat, sizeof(char) * copylen2);
    free(tmpformat);
    tmp[retlen] = 0;
    if (s != NULL)
        free(s);
    return tmp;
}

char * cnwn_strappend_realloc(char * s, const char * format, ...)
{
    va_list args;
    va_start(args, format);
    char * ret = cnwn_strappend_realloc_va(s, format, args);
    va_end(args);
    return ret;
}

char ** cnwn_strsplit(const char * s, int max_splits, const char * sep, const char * esc)
{
    int count = cnwn_strcount(s, 0, sep, esc);
    if (max_splits >= 0)
        count = CNWN_MIN(count, max_splits);
    char ** ret = malloc(sizeof(char *) * (count + 2));
    int seplen = cnwn_strlen(sep);
    if (count <= 0 || seplen <= 0) {
        ret[0] = cnwn_strdup(s != NULL ? s : "");
        ret[1] = NULL;
        return ret;
    }
    int last = 0;
    int index = cnwn_strfind(s, 0, sep, esc);
    int countr = 0;
    while (index >= 0 && countr < count) {
        int len = CNWN_MAX(0, index - last);
        ret[countr] = malloc(sizeof(char) * (len + 1));
        if (len > 0)
            memcpy(ret[countr], s + last, sizeof(char) * len);
        ret[countr][len] = 0;
        last = index + seplen;
        countr++;
        index = cnwn_strfind(s, last, sep, esc);
    }
    int len = cnwn_strlen(s + last);
    ret[countr] = malloc(sizeof(char) * (len + 1));
    if (len > 0)
        memcpy(ret[countr], s + last, sizeof(char) * len);
    ret[countr][len] = 0;
    countr++;
    ret[countr] = NULL;
    return ret;
}

char ** cnwn_strspliti(const char * s, int max_splits, const char * sep, const char * esc)
{
    int count = cnwn_strcount(s, 0, sep, esc);
    if (max_splits >= 0)
        count = CNWN_MIN(count, max_splits);
    char ** ret = malloc(sizeof(char *) * (count + 2));
    int seplen = cnwn_strlen(sep);
    if (count <= 0 || seplen <= 0) {
        ret[0] = cnwn_strdup(s != NULL ? s : "");
        ret[1] = NULL;
        return ret;
    }
    int last = 0;
    int index = cnwn_strfindi(s, 0, sep, esc);
    int countr = 0;
    while (index >= 0 && countr < count) {
        int len = CNWN_MAX(0, index - last);
        ret[countr] = malloc(sizeof(char) * (len + 1));
        if (len > 0)
            memcpy(ret[countr], s + last, sizeof(char) * len);
        ret[countr][len] = 0;
        last = index + seplen;
        countr++;
        index = cnwn_strfindi(s, last, sep, esc);
    }
    int len = cnwn_strlen(s + last);
    ret[countr] = malloc(sizeof(char) * (len + 1));
    if (len > 0)
        memcpy(ret[countr], s + last, sizeof(char) * len);
    ret[countr][len] = 0;
    countr++;
    ret[countr] = NULL;
    return ret;
}

int cnwn_strjoin(char * r, int max_size, char ** strings, const char * sep)
{
    int offset = 0;
    if (strings != NULL) {
        for (int i = 0; strings[i] != NULL && offset < max_size - 1; i++) {
            offset += cnwn_strcpy(r + offset, max_size - offset, strings[i], -1);
            if (strings[i + 1] != NULL) 
                offset += cnwn_strcpy(r + offset, max_size - offset, sep, -1);
        }
    }
    if (r != NULL && max_size > 0)
        r[offset] = 0;
    return offset;
}

int cnwn_strings_len(char ** strings)
{
    int len = 0;
    if (strings != NULL) 
        while (strings[len] != NULL)
            len++;
    return len;
}

char ** cnwn_strings_cat(char ** strings, char ** other)
{
    int counto = cnwn_strings_len(other);
    if (counto > 0) {
        int counts = cnwn_strings_len(strings);
        char ** ret = realloc(strings, sizeof(char *) * (counto + counts + 1));
        for (int i = 0; i < counto; i++)
            ret[i + counts] = other[i];
        ret[counto + counts] = NULL;
        return ret;
    }
    return strings;
}

void cnwn_strings_free(char ** strings)
{
    if (strings != NULL) {
        for (char ** p = strings; *p != NULL; p++)
            free(*p);
        free(strings);
    }
}

int cnwn_strintbase(const char * s)
{
    if (s != NULL && s[0] != 0) {
        int offset = (s[0] == '+' || s[0] == '-' ? 1 : 0);
        if ((s[offset] == '0' && (s[offset + 1] == 'd' || s[offset + 1] == 'D'))
            || (s[offset] >= '0' && s[offset] <= '9' && (s[offset + 1] == 0 || (s[offset + 1] >= '0' && s[offset + 1] <= '9'))))
            return 10;
        else if ((s[offset] == '$' || (s[offset] == '0' && (s[offset + 1] == 'x' || s[offset + 1] == 'X')))
                 && ((s[offset + 2] >= '0' && s[offset + 2] <= '9') || (s[offset + 2] >= 'a' && s[offset + 2] <= 'f') || (s[offset + 2] >= 'A' && s[offset + 2] <= 'F')))
            return 16;
        else if ((s[offset] == '@' || (s[offset] == '0' && (s[offset + 1] == 'o' || s[offset + 1] == 'O')))
                 && (s[offset + 2] >= '0' && s[offset + 2] <= '7'))
            return 8;
        else if ((s[offset] == '%' || (s[offset] == '0' && (s[offset + 1] == 'b' || s[offset + 1] == 'B')))
                 && (s[offset + 2] >= '0' && s[offset + 2] <= '1'))
            return 2;
    }
    return 0;
}

#define CNWN_DEFINE_INT_FUNC(funcsuffix, inttype, intmax, intmin) \
    bool cnwn_str##funcsuffix(const char * s, int base, inttype * ret_value) \
    { \
        if (cnwn_strisblank(s)) \
            return false; \
        if (base <= 0) \
            base = cnwn_strintbase(s); \
        if (base == 10 || base == 16 || base == 8 || base == 2) { \
            int start = (s[0] == '+' || s[0] == '-' ? 1 : 0); \
            if (base == 16)  \
                start += (s[start] == '$' ? 1 : (s[start] == '0' && (s[start + 1] == 'x' || s[start + 1] == 'X')) ? 2 : 0); \
            else if (base == 8)  \
                start += (s[start] == '@' ? 1 : (s[start] == '0' && (s[start + 1] == 'o' || s[start + 1] == 'O')) ? 2 : 0); \
            else if (base == 2)  \
                start += (s[start] == '%' ? 1 : (s[start] == '0' && (s[start + 1] == 'b' || s[start + 1] == 'B')) ? 2 : 0); \
            int len = start; \
            while (s[len] != 0) \
                len++; \
            inttype multiplier = (s[0] == '-' ? -1 : 1); \
            inttype result = 0; \
            int offset = len - 1; \
            while (offset >= start) { \
                inttype digitvalue = 0; \
                if (base == 10) { \
                    if (s[offset] >= '0' && s[offset] <= '9') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else if (base == 16) { \
                    if (s[offset] >= '0' && s[offset] <= '9') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else if (s[offset] >= 'a' && s[offset] <= 'f') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'a') + 10; \
                    else if (s[offset] >= 'A' && s[offset] <= 'F') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'A') + 10; \
                    else \
                        return false; \
                } else if (base == 8) { \
                    if (s[offset] >= '0' && s[offset] <= '7') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else if (base == 2) { \
                    if (s[offset] >= '0' && s[offset] <= '1') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else \
                    return false; \
                inttype value = digitvalue * multiplier; \
                if (value / multiplier != digitvalue) \
                    return false; \
                if (multiplier > 0 && value > intmax - result) \
                    return false; \
                else if (multiplier < 0 && value < intmin - result) \
                    return false; \
                result += value; \
                if (offset >= start) { \
                    inttype premult = multiplier * (inttype)base; \
                    if (premult / (inttype)base != multiplier) { \
                        offset--; \
                        while (offset >= start)  \
                            if (s[offset--] != '0')  \
                                return false; \
                        break; \
                    } \
                    multiplier = premult; \
                } \
                offset--; \
            } \
            if (ret_value != NULL) \
                *ret_value = result; \
            return true; \
        } \
        return false; \
    } \
    inttype cnwn_strto##funcsuffix(const char * s, int base, inttype errvalue) \
    { \
        inttype ret; \
        if (cnwn_str##funcsuffix(s, base, &ret)) \
            return ret; \
        return errvalue; \
    }

#define CNWN_DEFINE_UINT_FUNC(funcsuffix, inttype, intmax) \
    bool cnwn_str##funcsuffix(const char * s, int base, inttype * ret_value) \
    { \
        if (cnwn_strisblank(s)) \
            return false; \
        if (base <= 0) \
            base = cnwn_strintbase(s); \
        if (base == 10 || base == 16 || base == 8 || base == 2) { \
            int start = (s[0] == '+' ? 1 : 0); \
            if (base == 16)  \
                start += (s[start] == '$' ? 1 : (s[start] == '0' && (s[start + 1] == 'x' || s[start + 1] == 'X')) ? 2 : 0); \
            else if (base == 8)  \
                start += (s[start] == '@' ? 1 : (s[start] == '0' && (s[start + 1] == 'o' || s[start + 1] == 'O')) ? 2 : 0); \
            else if (base == 2)  \
                start += (s[start] == '%' ? 1 : (s[start] == '0' && (s[start + 1] == 'b' || s[start + 1] == 'B')) ? 2 : 0); \
            int len = start; \
            while (s[len] != 0) \
                len++; \
            inttype multiplier = 1; \
            inttype result = 0; \
            int offset = len - 1; \
            while (offset >= start) { \
                inttype digitvalue = 0; \
                if (base == 10) { \
                    if (s[offset] >= '0' && s[offset] <= '9') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else if (base == 16) { \
                    if (s[offset] >= '0' && s[offset] <= '9') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else if (s[offset] >= 'a' && s[offset] <= 'f') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'a') + 10; \
                    else if (s[offset] >= 'A' && s[offset] <= 'F') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'A') + 10; \
                    else \
                        return false; \
                } else if (base == 8) { \
                    if (s[offset] >= '0' && s[offset] <= '7') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else if (base == 2) { \
                    if (s[offset] >= '0' && s[offset] <= '1') \
                        digitvalue = ((inttype)s[offset]) - ((inttype)'0'); \
                    else \
                        return false; \
                } else \
                    return false; \
                inttype value = digitvalue * multiplier; \
                if (value / multiplier != digitvalue) \
                    return false; \
                if (value > intmax - result) \
                    return false; \
                result += value; \
                if (offset >= start) { \
                    inttype premult = multiplier * (inttype)base; \
                    if (premult / (inttype)base != multiplier) { \
                        offset--; \
                        while (offset >= start)  \
                            if (s[offset--] != '0')  \
                                return false; \
                        break; \
                    } \
                    multiplier = premult; \
                } \
                offset--; \
            } \
            if (ret_value != NULL) \
                *ret_value = result; \
            return true; \
        } \
        return false; \
    } \
    inttype cnwn_strto##funcsuffix(const char * s, int base, inttype errvalue) \
    { \
        inttype ret; \
        if (cnwn_str##funcsuffix(s, base, &ret)) \
            return ret; \
        return errvalue; \
    }

CNWN_DEFINE_INT_FUNC(short, short, SHRT_MAX, SHRT_MIN)
CNWN_DEFINE_UINT_FUNC(ushort, unsigned short, USHRT_MAX)
CNWN_DEFINE_INT_FUNC(int, int, INT_MAX, INT_MIN)
CNWN_DEFINE_UINT_FUNC(uint, unsigned int, UINT_MAX)
CNWN_DEFINE_INT_FUNC(long, long, LONG_MAX, LONG_MIN)
CNWN_DEFINE_UINT_FUNC(ulong, unsigned long, ULONG_MAX)
CNWN_DEFINE_INT_FUNC(longlong, long long, LLONG_MAX, LLONG_MIN)
CNWN_DEFINE_UINT_FUNC(ulonglong, unsigned long long, ULLONG_MAX)
CNWN_DEFINE_INT_FUNC(int8, int8_t, INT8_MAX, INT8_MIN)
CNWN_DEFINE_UINT_FUNC(uint8, uint8_t, UINT8_MAX)
CNWN_DEFINE_INT_FUNC(int16, int16_t, INT16_MAX, INT16_MIN)
CNWN_DEFINE_UINT_FUNC(uint16, uint16_t, UINT16_MAX)
CNWN_DEFINE_INT_FUNC(int32, int32_t, INT32_MAX, INT32_MIN)
CNWN_DEFINE_UINT_FUNC(uint32, uint32_t, UINT32_MAX)
CNWN_DEFINE_INT_FUNC(int64, int64_t, INT64_MAX, INT64_MIN)
CNWN_DEFINE_UINT_FUNC(uint64, uint64_t, UINT64_MAX)

bool cnwn_strdouble(const char * s, double * ret_value)
{
    if (s != NULL && s[0] != 0) {
        char * endptr = NULL;
        double r = strtod(s, &endptr);
        if (endptr != NULL && *endptr == 0) {
            if (ret_value != NULL)
                *ret_value = r;
            return 1;
        }
    }
    return false;
}

double cnwn_strtodouble(const char * s, double errvalue)
{
    double ret;
    if (cnwn_strdouble(s, &ret))
        return ret;
    return errvalue;
}

bool cnwn_strfloat(const char * s, float * ret_value)
{
    if (s != NULL && s[0] != 0) {
        char * endptr = NULL;
        float r = strtod(s, &endptr);
        if (endptr != NULL && *endptr == 0) {
            if (ret_value != NULL)
                *ret_value = r;
            return 1;
        }
    }
    return false;
}

float cnwn_strtofloat(const char * s, float errvalue)
{
    float ret;
    if (cnwn_strfloat(s, &ret))
        return ret;
    return errvalue;
}

bool cnwn_strbool(const char * s, bool * ret_value)
{
    if (s != NULL && s[0] != 0) {
        if (s[0] == '1' && s[1] == 0) {
            if (ret_value != NULL)
                *ret_value = true;
            return 1;
        }
        if (s[0] == '0' && s[1] == 0) {
            if (ret_value != NULL)
                *ret_value = false;
            return 1;
        }
        if ((s[0] == 'o' || s[0] == 'O')
            && (s[1] == 'n' || s[1] == 'N')
            && s[2] == 0) {
            if (ret_value != NULL)
                *ret_value = true;
            return 1;
        }
        if ((s[0] == 'n' || s[0] == 'N')
            && (s[1] == 'o' || s[1] == 'O')
            && s[2] == 0) {
            if (ret_value != NULL)
                *ret_value = false;
            return 1;
        }
        if ((s[0] == 'y' || s[0] == 'Y')
            && (s[1] == 'e' || s[1] == 'E')
            && (s[2] == 's' || s[2] == 'S')
            && s[3] == 0) {
            if (ret_value != NULL)
                *ret_value = true;
            return 1;
        }
        if ((s[0] == 'o' || s[0] == 'O')
            && (s[1] == 'f' || s[1] == 'F')
            && (s[2] == 'f' || s[2] == 'F')
            && s[3] == 0) {
            if (ret_value != NULL)
                *ret_value = false;
            return 1;
        }
        if ((s[0] == 't' || s[0] == 'T')
            && (s[1] == 'r' || s[1] == 'R')
            && (s[2] == 'u' || s[2] == 'U')
            && (s[3] == 'e' || s[3] == 'E')
            && s[4] == 0) {
            if (ret_value != NULL)
                *ret_value = true;
            return 1;
        }
        if ((s[0] == 'f' || s[0] == 'F')
            && (s[1] == 'a' || s[1] == 'A')
            && (s[2] == 'l' || s[2] == 'L')
            && (s[3] == 's' || s[3] == 'S')
            && (s[4] == 'e' || s[4] == 'E')
            && s[5] == 0) {
            if (ret_value != NULL)
                *ret_value = false;
            return 1;
        }
    }
    return false;
}

bool cnwn_strtobool(const char * s, bool errvalue)
{
    bool ret;
    if (cnwn_strbool(s, &ret))
        return ret;
    return errvalue;
}
