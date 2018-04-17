#include "cnwn/regulexp.h"

#ifdef SOME_PLATFORM
#else
#include <regex.h>
struct cnwn_Regexp_s { regex_t regex; };
#endif

int cnwn_regexp_init(cnwn_Regexp * regexp, const char * s, bool extended)
{
    memset(regexp, 0, sizeof(cnwn_Regexp));
#ifdef SOME_PLATFORM
#else
    int flags = REG_ICASE;
    if (extended)
        flags |= REG_EXTENDED;
    int regret = regcomp(&regexp->regex, s, flags);
    if (regret != 0) {
        char tmps[8192];
        regerror(regret, &regexp->regex, tmps, sizeof(tmps));
        cnwn_set_error("%s", tmps);
        return -1;
    }
    return 0;
#endif
}

void cnwn_regexp_deinit(cnwn_Regexp * regexp)
{
#ifdef SOME_PLATFORM
#else
    regfree(&regexp->regex);
#endif
}

bool cnwn_regexp_match(const cnwn_Regexp * regexp, const char * s)
{
    if (s == NULL)
        s = "";
#ifdef SOME_PLATFORM
#else
    return regexec(&regexp->regex, s, 0, NULL, 0) == 0;
#endif
}

int cnwn_regexp_array_init(cnwn_RegexpArray * regexp_array, bool extended, const char * regexps[])
{
    memset(regexp_array, 0, sizeof(cnwn_RegexpArray));
    if (regexps != NULL) {
        const char * as;
        int count = 0;
        while ((as = regexps[count]) != NULL) 
            count++;
        cnwn_Regexp * regs = count > 0 ? malloc(sizeof(cnwn_Regexp) * count) : NULL;
        int index = 0;
        while ((as = regexps[index]) != NULL && index < count) {
            int regret = cnwn_regexp_init(regs + index, as, extended);
            if (regret < 0) 
                break;
            index++;
        }    
        if (index < count) {
            cnwn_set_error("regexp arg %d failed, %s", index + 1, cnwn_get_error());
            for (int i = 0; i < index; i++)
                cnwn_regexp_deinit(regs + index);
            free(regexps);
            return -1;
        }
        regexp_array->length = index;
        regexp_array->regexps = regs;
        return index;
    }
    return 0;
}

int cnwn_regexp_array_init_var(cnwn_RegexpArray * regexp_array, bool extended, ...)
{
    memset(regexp_array, 0, sizeof(cnwn_RegexpArray));
    va_list args, args_copy;
    va_start(args, extended);
    va_copy(args_copy, args);
    char * as;
    int count = 0;
    while ((as = va_arg(args_copy, char *)) != NULL) 
        count++;
    va_end(args_copy);
    cnwn_Regexp * regexps = count > 0 ? malloc(sizeof(cnwn_Regexp) * count) : NULL;
    int index = 0;
    while ((as = va_arg(args, char *)) != NULL && index < count) {
        int regret = cnwn_regexp_init(regexps + index, as, extended);
        if (regret < 0) 
            break;
        index++;
    }    
    va_end(args);
    if (index < count) {
        cnwn_set_error("regexp arg %d failed, %s", index + 1, cnwn_get_error());
        for (int i = 0; i < index; i++)
            cnwn_regexp_deinit(regexps + index);
        free(regexps);
        return -1;
    }
    regexp_array->length = index;
    regexp_array->regexps = regexps;
    return index;
}

void cnwn_regexp_array_deinit(cnwn_RegexpArray * regexp_array)
{
    for (int i = 0; i < regexp_array->length; i++)
        cnwn_regexp_deinit(regexp_array->regexps + i);
    free(regexp_array->regexps);
}

bool cnwn_regexp_array_match_any(const cnwn_RegexpArray * regexp_array, const char * s)
{
    if (regexp_array->length <= 0)
        return true;
    for (int i = 0; i < regexp_array->length; i++)
        if (cnwn_regexp_match(regexp_array->regexps + i, s))
            return true;
    return false;
}

bool cnwn_regexp_array_match_all(const cnwn_RegexpArray * regexp_array, const char * s)
{
    if (regexp_array->length <= 0)
        return true;
    if (regexp_array->length > 0) {
        for (int i = 0; i < regexp_array->length; i++)
            if (!cnwn_regexp_match(regexp_array->regexps + i, s))
                return false;
        return true;
    }
    return false;
}
