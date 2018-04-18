/**
 * @file cliopt.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_CLIOPT_H
#define CNWN_CLIOPT_H

#include "cnwn/common.h"

/**
 * ANSI color red.
 **/
#define CNWN_ANSI_COLOR_RED "\x1b[91m"

/**
 * ANSI color green.
 **/
#define CNWN_ANSI_COLOR_GREEN "\x1b[92m"

/**
 * ANSI color yellow.
 **/
#define CNWN_ANSI_COLOR_YELLOW "\x1b[93m"

/**
 * ANSI color blue.
 **/
#define CNWN_ANSI_COLOR_BLUE "\x1b[94m"

/**
 * ANSI color magenta.
 **/
#define CNWN_ANSI_COLOR_MAGENTA "\x1b[95m"

/**
 * ANSI color cyan.
 **/
#define CNWN_ANSI_COLOR_CYAN "\x1b[96m"

/**
 * ANSI color white.
 **/
#define CNWN_ANSI_COLOR_WHITE "\x1b[97m"

/**
 * ANSI color black.
 **/
#define CNWN_ANSI_COLOR_BLACK "\x1b[30m"

/**
 * ANSI color grey.
 **/
#define CNWN_ANSI_COLOR_GREY "\x1b[37m"

/**
 * ANSI color reset.
 **/
#define CNWN_ANSI_COLOR_RESET "\x1b[0m"

/**
 * Check whether or not an option is a short option (single dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_ARG_IS_SHORT(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] != '-' && (arg)[1] != 0 && (arg)[2] == 0)

/**
 * Check whether or not an option is a long option (double dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_ARG_IS_LONG(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] == '-'  && (arg)[2] != '=' && (arg)[2] != 0)

/**
 * Check whether or not an option is an option or regular argument.
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_ARG_IS_OPTION(arg) (CNWN_CLI_OPTION_ARG_IS_SHORT(arg) || CNWN_CLI_OPTION_ARG_IS_LONG(arg))

/**
 * The option is invalid.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_INVALID -1

/**
 * The option is missing an argument.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_MISSING -2

/**
 * @see struct cnwn_CliOption_s
 */
typedef struct cnwn_CliOption_s cnwn_CliOption;

/**
 * Cli option definition.
 */
struct cnwn_CliOption_s {

    /**
     * Option short key.
     */
    const char * skey;

    /**
     * Option long key.
     */
    const char * lkey;
    
    /**
     * What the argument to the option should be named, NULL for no argument.
     */
    const char * argname;

    /**
     * A help string.
     */
    const char * help;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Turn a cli option into a string.
 * @param option The cli option.
 * @param max_size The maximum size of the returned string including zero terminator.
 * @returns The length of the returned string excluding zero terminator.
 */
extern CNWN_PUBLIC int cnwn_cli_option_to_string(const cnwn_CliOption * option, int max_size, char * ret_s);

/**
 * Find an option based on an argument.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param arg The argument.
 * @returns The index of the found option or -1 for no match.
 */
extern CNWN_PUBLIC int cnwn_cli_option_find(const cnwn_CliOption * options, const char * arg);

/**
 * Find an option based on an argument.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param arg The argument.
 * @returns The cli option or NULL for no match.
 */
extern CNWN_PUBLIC const cnwn_CliOption * cnwn_cli_option_get(const cnwn_CliOption * options, const char * arg);

/**
 * Find an option based on an argument.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param argindex The index of the argument.
 * @param argc The number of available arguments.
 * @param argv The arguments.
 * @param[out] ret_optindex Return the index of the parsed option or -1 if there was no match, pass NULL to omit.
 * @param[out] ret_optarg Return the option's argument or NULL if no argument was found, pass NULL to omit.
 * @returns The number of consumed argument, zero if there are no more arguments or a negative value for an invalid option. The negative return value can be matched with CNWN_CLI_OPTION_INVALID and CNWN_CLI_OPTION_MISSING for better detail on what went wrong.
 */
extern CNWN_PUBLIC int cnwn_cli_options_parse(const cnwn_CliOption * options, int argindex, int argc, char * argv[], int * ret_optindex, const char ** ret_optarg);

/**
 * Print help text for options.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param prefix Prefix every printed line with this string (NULL is OK).
 */
extern CNWN_PUBLIC void cnwn_cli_options_print_help(const cnwn_CliOption * options, const char * prefix);

#ifdef __cplusplus
}
#endif

#endif
