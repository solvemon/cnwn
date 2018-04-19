/**
 * @file cliopt.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_CLIOPT_H
#define CNWN_CLIOPT_H

#include "cnwn/common.h"

/**
 * Get an ANSI escape sequence for text color.
 * @param c The color enum.
 * @returns A pointer to a string with the escape sequence or an empty string if @p c is invalid or cnwn_cli_has_color() returns false.
 */
#define CNWN_CLI_COLOR(c) ((c) >= CNWN_CLI_COLOR_NONE && (c) < CNWN_MAX_CLI_COLOR && cnwn_cli_has_color() ? CNWN_CLI_COLORS[(c)] : "")

/**
 * Gray = grey.
 */
#define CNWN_CLI_COLOR_GRAY CNWN_CLI_COLOR_GREY

/**
 * Normal = none (reset)
 */
#define CNWN_CLI_COLOR_NORMAL CNWN_CLI_COLOR_NONE

/**
 * Emphasis = white (bright)
 */
#define CNWN_CLI_COLOR_EMPHASIS CNWN_CLI_COLOR_WHITE

/**
 * Alert = red.
 */
#define CNWN_CLI_COLOR_ALERT CNWN_CLI_COLOR_RED

/**
 * Number = cyan.
 */
#define CNWN_CLI_COLOR_NUMBER CNWN_CLI_COLOR_CYAN

/**
 * Path = magenta.
 */
#define CNWN_CLI_COLOR_PATH CNWN_CLI_COLOR_MAGENTA

/**
 * Entry = green.
 */
#define CNWN_CLI_COLOR_ENTRY CNWN_CLI_COLOR_GREEN

/**
 * Details = grey.
 */
#define CNWN_CLI_COLOR_DETAILS CNWN_CLI_COLOR_GREY

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
 * A list of colors.
 */
enum cnwn_CliColor_e {

    /**
     * No color (reset color code).
     */
    CNWN_CLI_COLOR_NONE = 0,

    /**
     * The whitest color available to the terminal.
     */
    CNWN_CLI_COLOR_WHITE,

    /**
     * A "bright shade of black.
     */
    CNWN_CLI_COLOR_GREY,

    /**
     * Red.
     */
    CNWN_CLI_COLOR_RED,

    /**
     * Green.
     */
    CNWN_CLI_COLOR_GREEN,

    /**
     * Blue.
     */
    CNWN_CLI_COLOR_BLUE,

    /**
     * Yellow.
     */
    CNWN_CLI_COLOR_YELLOW,

    /**
     * Cyan
     */
    CNWN_CLI_COLOR_CYAN,
    
    /**
     * Magenta.
     */
    CNWN_CLI_COLOR_MAGENTA,
    
    /**
     * Max enum.
     */
    CNWN_MAX_CLI_COLOR
};


/**
 * @see enum cnwn_CliColor_e
 */
typedef enum cnwn_CliColor_e cnwn_CliColor;

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
 * Some predefined ANSI colors.
 */
extern CNWN_PUBLIC const char * CNWN_CLI_COLORS[CNWN_MAX_CLI_COLOR];

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

/**
 * Check it the terminal has ANSI color support.
 * @returns True if the we have color support on stdout, false if not.
 */
extern CNWN_PUBLIC bool cnwn_cli_has_color(void);

#ifdef __cplusplus
}
#endif

#endif
