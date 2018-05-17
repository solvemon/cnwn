#include "cnwn/regexp.h"

#include <regex.h>
struct cnwn_Regexp_s { regex_t regex; };

int cnwn_regexp_init(cnwn_Regexp * regexp, const char * re)
{
    regex_t regex;
    int ret = regcomp(&regex, re, REG_ICASE);
    if (ret != 0) {
        char tmps[1024];
        regerror(ret, &regex, tmps, sizeof(tmps));
        cnwn_set_error("%s", tmps);
        return -1;
    }
    regexp->regex = regex;
    return 0;
}
        
void cnwn_regexp_deinit(cnwn_Regexp * regexp)
{
    regfree(&regexp->regex);
}

bool cnwn_regexp_match(const cnwn_Regexp * regexp, const char * s)
{
    int ret = regexec(&regexp->regex, s, 0, NULL, 0);
    return ret == 0;
}

cnwn_RegexpArray * cnwn_regexp_array_new(int num_strings, const char ** strings)
{
    num_strings = CNWN_MAX(0, num_strings);
    cnwn_RegexpArray array = {0};
    array.length = num_strings;
    if (num_strings > 0 && strings != NULL) {
        array.regexps = malloc(sizeof(cnwn_Regexp) * num_strings);
        int count = 0;
        for (int i = 0; i < num_strings; i++) {
            if (!cnwn_strisblank(strings[i])) {
                if (cnwn_regexp_init(array.regexps + count, strings[i]) < 0) {
                    cnwn_set_error("%s (regexp #%d)", cnwn_get_error(), i);
                    free(array.regexps);
                    return NULL;
                }
                count++;
            }
        }
        array.length = count;
        if (count > 0) 
            array.regexps = realloc(array.regexps, sizeof(cnwn_Regexp) * count);
        else {
            free(array.regexps);
            array.regexps = NULL;
        }
    }
    cnwn_RegexpArray * ret = malloc(sizeof(cnwn_RegexpArray));
    *ret = array;
    return ret;
}

cnwn_RegexpArray * cnwn_regexp_array_new2(const cnwn_StringArray * string_array)
{
    cnwn_RegexpArray array = {0};
    int num_strings = cnwn_array_get_length(string_array);
    array.length = num_strings;
    if (num_strings > 0) {
        array.regexps = malloc(sizeof(cnwn_Regexp) * num_strings);
        int count = 0;
        for (int i = 0; i < num_strings; i++) {
            const char * s = cnwn_string_array_get(string_array, i);
            if (!cnwn_strisblank(s)) {
                if (cnwn_regexp_init(array.regexps + count, s) < 0) {
                    cnwn_set_error("%s (regexp #%d)", cnwn_get_error(), i);
                    free(array.regexps);
                    return NULL;
                }
                count++;
            }
        }
        array.length = count;
        if (count > 0) 
            array.regexps = realloc(array.regexps, sizeof(cnwn_Regexp) * count);
        else {
            free(array.regexps);
            array.regexps = NULL;
        }
    }
    cnwn_RegexpArray * ret = malloc(sizeof(cnwn_RegexpArray));
    *ret = array;
    return ret;
}

void cnwn_regexp_array_free(cnwn_RegexpArray * regexp_array)
{
    if (regexp_array->regexps != NULL) {
        for (int i = 0; i < regexp_array->length; i++)
            cnwn_regexp_deinit(regexp_array->regexps + i);
        free(regexp_array->regexps);
    }
    free(regexp_array);
}

int cnwn_regexp_array_length(const cnwn_RegexpArray * regexp_array)
{
    return regexp_array->length;
}

int cnwn_regexp_array_append(cnwn_RegexpArray * regexp_array, const char * re)
{
    cnwn_Regexp regexp = {0};
    int ret = cnwn_regexp_init(&regexp, re);
    if (ret < 0)
        return -1;
    regexp_array->regexps = realloc(regexp_array->regexps, sizeof(cnwn_Regexp) * (regexp_array->length + 1));
    regexp_array->regexps[regexp_array->length] = regexp;
    regexp_array->length++;
    return regexp_array->length;
}

bool cnwn_regexp_array_match_all(const cnwn_RegexpArray * regexp_array, const char * s)
{
    for (int i = 0; i < regexp_array->length; i++)
        if (!cnwn_regexp_match(regexp_array->regexps + i, s))
            return false;
    return true;
}

bool cnwn_regexp_array_match_any(const cnwn_RegexpArray * regexp_array, const char * s)
{
    for (int i = 0; i < regexp_array->length; i++)
        if (cnwn_regexp_match(regexp_array->regexps + i, s))
            return true;
    return regexp_array->length <= 0;
}
