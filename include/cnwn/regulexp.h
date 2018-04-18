/**
 * @file regulexp.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_REGULEXP_H
#define CNWN_REGULEXP_H

#include "cnwn/common.h"

/**
 * A regular expression handle, the definition is platform dependent.
 *
 * POSIX:
 * struct cnwn_Regexp_s { regex_t regex; };
 */
typedef struct cnwn_Regexp_s cnwn_Regexp;

/**
 * @see struct cnwn_RegexpArray_s
 */
typedef struct cnwn_RegexpArray_s cnwn_RegexpArray;

/**
 * Array of regular expressions.
 */
struct cnwn_RegexpArray_s {

    /**
     * The number of regular expression.
     */
    int length;

    /**
     * The regexps.
     */
    cnwn_Regexp * regexps;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize a regular expression.
 * @param regexp The regular expression to initialize.
 * @param s The regular expression.
 * @param extended True for extended regexps.
 * @returns Zero on sucess and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_regexp_init(cnwn_Regexp * regexp, const char * s, bool extended);

/**
 * Deinitialize a regular expression.
 * @param regexp The regular expression to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_regexp_deinit(cnwn_Regexp * regexp);

/**
 * See if a string matches a regular expression.
 * @param regexp The regular expression to use.
 * @param s The string to match for.
 * @returns True if there is a match, false if not.
 */
extern CNWN_PUBLIC bool cnwn_regexp_match(const cnwn_Regexp * regexp, const char * s);

/**
 * Initialize an array of regular expressions.
 * @param regexp_array The array struct to initialize.
 * @param extended True for extended regexps.
 * @param regexps The strings with regular expressions and terminated with a NULL string.
 * @returns The number of regexps in the intialized array or a negative value on error.
 */
extern CNWN_PUBLIC int cnwn_regexp_array_init(cnwn_RegexpArray * regexp_array, bool extended, const char * regexps[]);

/**
 * Initialize an array of regular expressions.
 * @param regexp_array The array struct to initialize.
 * @param extended True for extended regexps.
 * @returns The number of regexps in the intialized array or a negative value on error.
 * @note The variable arguments must be strings with regular expressions and terminated with a NULL argument.
 */
extern CNWN_PUBLIC int cnwn_regexp_array_init_var(cnwn_RegexpArray * regexp_array, bool extended, ...);

/**
 * Deinitialize regexp array.
 * @param regexp_array The regular expression to free.
 */
extern CNWN_PUBLIC void cnwn_regexp_array_deinit(cnwn_RegexpArray * regexp_array);

/**
 * Match the string against all of the arrays regular expressions using logical OR.
 * @param regexp_array The regular expression to use.
 * @param s The string to match for.
 * @returns True if @p s matches ANY regexp in the array, false otherwise.
 * @note An array with zero length will always return true.
 */
extern CNWN_PUBLIC bool cnwn_regexp_array_match_any(const cnwn_RegexpArray * regexp_array, const char * s);

/**
 * Match the string against all of the arrays regular expressions using logical AND.
 * @param regexp_array The regular expression to use.
 * @param s The string to match for.
 * @returns True if @p s matches ALL regexp in the array, false otherwise.
 * @note An array with zero length will always return true.
 */
extern CNWN_PUBLIC bool cnwn_regexp_array_match_all(const cnwn_RegexpArray * regexp_array, const char * s);

#ifdef __cplusplus
}
#endif

#endif
