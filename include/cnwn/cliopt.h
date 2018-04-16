/**
 * @file cliopt.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_CLIOPT_H
#define CNWN_CLIOPT_H

#include "cnwn/common.h"

/**
 * Check whether or not an option is a short option (single dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_IS_SHORT(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] != '-' && (arg)[1] != 0 && (arg)[2] == 0)

/**
 * Check whether or not an option is a long option (double dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_IS_LONG(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] == '-' && (arg)[2] != 0)

/**
 * The option is recognized as a normal argument.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_ARG -1

/**
 * An invalid option was specified.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_INVALID -2

/**
 * A valid option expected an argument, but none was found.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_INCOMPLETE -3

/**
 * @see struct cnwn_CliOption_s
 */
typedef struct cnwn_CliOption_s cnwn_CliOption;

/**
 * Cli option definition.
 */
struct cnwn_CliOption_s {

    /**
     * Option key.
     */
    const char * key;
    
    /**
     * True if the option requires an argument.
     */
    bool requires_argument;

    /**
     * A help string.
     */
    const char * help;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parse some cli options.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param arg The argument.
 * @param next The argument following @p arg or NULL if there are no more arguments.
 * @param[out] ret_arg Return an argument, either to an option (if the function returns >= 0, can be NULL if the option has no argument) or as a normal non-option argument if the function returns @ref CNWN_CLI_OPTION_ARG.
 * @returns A zero or possitive option index of a valid option or a negative value for non-options, see details for result codes.
 * @note To check whether you need to skip one argument in argv (for long options that return non-NULL @p ret_arg) use the CNWN_CLI_OPTION_IS_SHORT() macro.
 *
 * Result codes:
 * - >= 0 is a valid option index.
 * CNWN_CLI_OPTION_ARG to indicate a normal argument.
 * CNWN_CLI_OPTION_INVALID to indicate an invalid option.
 * CNWN_CLI_OPTION_INCOMPLETE to indicate a valid option with missing argument.
 */
extern CNWN_PUBLIC int cnwn_parse_cli_option(const cnwn_CliOption * options, const char * arg, const char * next, const char ** ret_arg);

/**
 * Print help text for options.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param prefix Prefix every printed line with this string (NULL is OK).
 */
extern CNWN_PUBLIC void cnwn_print_cli_options(const cnwn_CliOption * options, const char * prefix);

#ifdef __cplusplus
}
#endif

#endif
