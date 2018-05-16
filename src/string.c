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
