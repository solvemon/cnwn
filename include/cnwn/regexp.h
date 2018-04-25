/**
 * @file regexp.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_REGEXP_H
#define CNWN_REGEXP_H

#include "cnwn/common.h"
#include "cnwn/string.h"
#include "cnwn/options.h"

/**
 * Definition depends on platform:
 * - Right now all platforms use: struct cnwn_Regexp_s { regex_t regex; };
 */
typedef struct cnwn_Regexp_s cnwn_Regexp;

/**
 * @see struct cnwn_RegexpArray_s
 */
typedef struct cnwn_RegexpArray_s cnwn_RegexpArray;

/**
 * An array of regular expressions.
 */
struct cnwn_RegexpArray_s {

    /**
     * The number of regular expressions in the array.
     */
    int length;

    /**
     * The regular expressions.
     */
    cnwn_Regexp * regexps;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize (and compile) a regular expression.
 * @param regexp The regexp struct to initialize.
 * @param re The regular expression.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_regexp_init(cnwn_Regexp * regexp, const char * re);

/**
 * Deinitialize a regular expression.
 * @param regexp The regexp to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_regexp_deinit(cnwn_Regexp * regexp);

/**
 * Match a string against the regular expression.
 * @param regexp The regexp to match against.
 * @param s The string to match.
 * @returns True if a match is found, false if not.
 */
extern CNWN_PUBLIC bool cnwn_regexp_match(const cnwn_Regexp * regexp, const char * s);

/**
 * Create a new regexp array from some strings.
 * @param num_strings The number of strings.
 * @param strings The strings.
 * @returns The new regexp array or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC cnwn_RegexpArray * cnwn_regexp_array_new(int num_strings, const char ** strings);

/**
 * Create a new regexp array from arguments.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @returns The new regexp array or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC cnwn_RegexpArray * cnwn_regexp_array_new_args(int argc, char * argv[]);

/**
 * Free a regexp array.
 * @param regexp_array The regexp array to free.
 */
extern CNWN_PUBLIC void cnwn_regexp_array_free(cnwn_RegexpArray * regexp_array);

/**
 * Get the length of a regexp array.
 * @param regexp_array The regexp array to get the length from.
 * @returns The length (i.e number of regular expressions in the array).
 */
extern CNWN_PUBLIC int cnwn_regexp_array_length(const cnwn_RegexpArray * regexp_array);

/**
 * Append a regexp to the array.
 * @param regexp_array The regexp array to append.
 * @param re The regular expression to append.
 * @returns The new length (i.e number of regular expressions in the array) or a negative error on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC int cnwn_regexp_array_append(cnwn_RegexpArray * regexp_array, const char * re);

/**
 * Match all (AND).
 * @param regexp_array The regexp array to match against.
 * @param s The string to match.
 * @returns True if all regular expressions in the array matches, false if not.
 * @note If the array is empty the function returns true.
 */
extern CNWN_PUBLIC bool cnwn_regexp_array_match_all(const cnwn_RegexpArray * regexp_array, const char * s);

/**
 * Match any (OR).
 * @param regexp_array The regexp array to match against.
 * @param s The string to match.
 * @returns True if any of the regular expressions in the array matches, false if not.
 * @note If the array is empty the function returns true.
 */
extern CNWN_PUBLIC bool cnwn_regexp_array_match_any(const cnwn_RegexpArray * regexp_array, const char * s);

#ifdef __cplusplus
}
#endif

#endif
