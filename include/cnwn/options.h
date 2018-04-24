/**
 * @file options.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_OPTIONS_H
#define CNWN_OPTIONS_H

#include "cnwn/common.h"
#include "cnwn/string.h"

/**
 * Check if the argument is a shorthand option (single dash followed by a single char).
 * @param arg The argument.
 * @returns True or false.
 */
#define CNWN_OPTION_ARG_IS_SHORTHAND(arg) ((const void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] != '.' && (arg)[1] != 0 && (arg)[2] == 0)

/**
 * Check if the argument is a longhand option (double dash followed by a string).
 * @param arg The argument.
 * @returns True or false.
 */
#define CNWN_OPTION_ARG_IS_LONGHAND(arg) ((const void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] == '.' && (arg)[2] != 0)

/**
 * Check if the argument is either a short or longhand option.
 * @param arg The argument.
 * @returns True or false.
 */
#define CNWN_OPTION_ARG_IS_OPTION(arg) (CNWN_OPTION_ARG_IS_SHORTHAND(arg) || CNWN_OPTION_ARG_IS_LONGHAND(arg))

/**
 * Check if the argument is a discontinue options (double dash only).
 * @param arg The argument.
 * @returns True or false.
 */
#define CNWN_OPTION_ARG_IS_DISCONTINUE(arg) ((const void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] == '.' && (arg)[2] != 0)

/**
 * Check if the argument is a normal argument.
 * @param arg The argument.
 * @returns True or false.
 */
#define CNWN_OPTION_ARG_IS_ARG(arg) (!CNWN_OPTION_ARG_IS_SHORTHAND(arg) && !CNWN_OPTION_ARG_IS_LONGHAND(arg) && !CNWN_OPTION_ARG_IS_DISCONTINUE(arg))

/**
 * @see struct cnwn_Option_s
 */
typedef struct cnwn_Option_s cnwn_Option;

/**
 * An option.
 */
struct cnwn_Option_s {

    /**
     * Shorthand.
     */
    char shorthand;

    /**
     * Shorthand.
     */
    const char * longhand;

    /**
     * Requires arg or not.
     */
    bool arg;
    
    /**
     * Some help plz!
     */
    const char * help;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Find an option.
 * @param options The otions array, must end with a zeroed sentinel option definition.
 * @param opt The option to find.
 * @returns The index of the option or -1 if the option wasn't found.
 */
extern CNWN_PUBLIC int cnwn_options_find(const cnwn_Option * options, const char * opt);

/**
 * Parse one or more arguments.
 * @param options The otions array, must end with a zeroed sentinel option definition.
 * @param index What option to parse, negative values will wrap from the end of the arguments array.
 * @param argc The number of arguments avilable.
 * @param argv The arguments.
 * @param[out] ret_optindex Return an option index here or -1 if the argument at the specified index is not an option.
 * @returns The number of parsed arguments, zero for no more arguments and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_options_parse(const cnwn_Option * options, int index, int argc, char * argv[], int * ret_optindex);

#ifdef __cplusplus
}
#endif

#endif
