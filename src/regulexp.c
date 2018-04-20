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

int cnwn_regexp_array_init(cnwn_RegexpArray * regexp_array, bool extended, int num_strings, const char ** strings)
{
    memset(regexp_array, 0, sizeof(cnwn_RegexpArray));
    int ret = 0;
    if (strings != NULL && num_strings > 0) {
        int regret = 0;
        cnwn_Regexp * regs = malloc(sizeof(cnwn_Regexp) * num_strings);
        int i;
        for (i = 0; i < num_strings; i++) {
            if (!cnwn_string_isempty(strings[i])) {
                regret = cnwn_regexp_init(regs + i, strings[i], extended);
                if (regret < 0) 
                    break;
                ret++;
            }
        }    
        if (regret < 0) {
            cnwn_set_error("regexp arg %d failed, %s", i + 1, cnwn_get_error());
            for (int j = 0; j < ret; j++)
                cnwn_regexp_deinit(regs + ret);
            free(regs);
            return -1;
        }
        regexp_array->length = ret;
        if (ret > 0)
            regexp_array->regexps = realloc(regs, sizeof(cnwn_Regexp) * ret);
        else {
            free(regexp_array->regexps);
            regexp_array->regexps = NULL;
        }
    }
    return ret;
}

int cnwn_regexp_array_init2(cnwn_RegexpArray * regexp_array, bool extended, const char ** strings)
{
    memset(regexp_array, 0, sizeof(cnwn_RegexpArray));
    int ret = 0;
    if (strings != NULL) {
        int count = cnwn_count_strings((char **)strings);
        return cnwn_regexp_array_init(regexp_array, extended, count, strings);
    }
    return ret;
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

cnwn_RegexpArray * cnwn_regexp_array_new(bool extended, int num_strings, const char ** strings)
{
    cnwn_RegexpArray ra;
    if (cnwn_regexp_array_init(&ra, extended, num_strings, strings) < 0)
        return NULL;
    cnwn_RegexpArray * ret = malloc(sizeof(cnwn_RegexpArray));
    *ret = ra;
    return ret;
}

cnwn_RegexpArray * cnwn_regexp_array_new2(bool extended, const char ** strings)
{
    cnwn_RegexpArray ra;
    if (cnwn_regexp_array_init2(&ra, extended, strings) < 0)
        return NULL;
    cnwn_RegexpArray * ret = malloc(sizeof(cnwn_RegexpArray));
    *ret = ra;
    return ret;
}

void cnwn_regexp_array_deinit(cnwn_RegexpArray * regexp_array)
{
    for (int i = 0; i < regexp_array->length; i++)
        cnwn_regexp_deinit(regexp_array->regexps + i);
    free(regexp_array->regexps);
}

void cnwn_regexp_array_free(cnwn_RegexpArray * regexp_array)
{
    cnwn_regexp_array_deinit(regexp_array);
    free(regexp_array);
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
