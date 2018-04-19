/**
 * @file erf_util.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_UTIL_H
#define CNWN_ERF_UTIL_H

#include "cnwn/erf.h"
#include "cnwn/cliopt.h"

/**
 * Normal = none (reset)
 * @param ok True to use a color false to return an empty string.
 */
#define CNWN_COLORIZE_NORMAL(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_NONE) : "")

/**
 * Emphasis = white (bright)
 */
#define CNWN_COLORIZE_EMPHASIS(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_WHITE) : "")

/**
 * Error = red.
 */
#define CNWN_COLORIZE_ERROR(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_RED) : "")

/**
 * Number = cyan.
 */
#define CNWN_COLORIZE_NUMBER(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_GREEN) : "")

/**
 * Path = magenta.
 */
#define CNWN_COLORIZE_PATH(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_MAGENTA) : "")

/**
 * Entry = green.
 */
#define CNWN_COLORIZE_ENTRY(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_YELLOW) : "")

/**
 * Details = grey.
 */
#define CNWN_COLORIZE_DETAILS(ok) ((ok) ? CNWN_CLI_COLOR(CNWN_CLI_COLOR_GREY) : "")

/**
 * Describes a CLI option.
 */
enum cnwn_ERFUtilOption_e {

    /**
     * Nothing.
     */
    CNWN_ERF_UTIL_OPTION_NONE = 0,

    /**
     * Help.
     */
    CNWN_ERF_UTIL_OPTION_HELP,

    /**
     * Version.
     */
    CNWN_ERF_UTIL_OPTION_VERSION,

    /**
     * Verbose.
     */
    CNWN_ERF_UTIL_OPTION_VERBOSE,
    
    /**
     * Ignore errors.
     */
    CNWN_ERF_UTIL_OPTION_IGNORE_ERRORS,

    /**
     * Color.
     */
    CNWN_ERF_UTIL_OPTION_COLOR,

    /**
     * Extended regexp.
     */
    CNWN_ERF_UTIL_OPTION_EXTENDED_REGEXP,

    /**
     * Force.
     */
    CNWN_ERF_UTIL_OPTION_FORCE,

    /**
     * Directory.
     */
    CNWN_ERF_UTIL_OPTION_DIRECTORY,

    /**
     * Set version.
     */
    CNWN_ERF_UTIL_OPTION_SET_VERSION,

    /**
     * XML.
     */
    CNWN_ERF_UTIL_OPTION_XML,

    /**
     * Temporary file location.
     */
    CNWN_ERF_UTIL_OPTION_TEMPORARY_FILE,
    
    /**
     * Max enum.
     */
    CNWN_MAX_ERF_UTIL_OPTION
};
    
/**
 * @see enum cnwn_ERFUtilOption_e
 */
typedef enum cnwn_ERFUtilOption_e cnwn_ERFUtilOption;

/**
 * @see struct cnwn_ERFUtilOptions_s
 */
typedef struct cnwn_ERFUtilOptions_s cnwn_ERFUtilOptions;

/**
 * @see struct cnwn_ERFUtilOptionError_s
 */
typedef struct cnwn_ERFUtilOptionError_s cnwn_ERFUtilOptionError;

/**
 * The options for all util operations.
 */
struct cnwn_ERFUtilOptions_s {

    /**
     * Help flag.
     */
    bool help;

    /**
     * Version flag.
     */
    bool version;

    /**
     * Verbose output.
     */
    bool verbose;

    /**
     * Ignore errors.
     */
    bool ignore_errors;
    
    /**
     * Use colored output.
     */
    bool color;

    /**
     * Use extended regexp.
     * @note Appiles to list and extract.
     */
    bool extended_regexp;
    
    /**
     * Force overwrites (don't ask!).
     * @note Appiles to extract, create, add and remove commands.
     */
    bool force;

    /**
     * Use a directory.
     * @note Appiles to extract only.
     */
    const char * directory;
    
    /**
     * ERF version.
     * @note Appiles to create command only.
     */
    cnwn_Version set_version;
    
    /**
     * Use XML output if possible
     * @note Appiles to extract command only.
     */
    bool xml;

    /**
     * Temporary file.
     * @note Appiles to add and remove commands.
     */
    const char * temporary_file;
    
};

/**
 * Give some information about the errors encountered while parsing cli arguments.
 */
struct cnwn_ERFUtilOptionError_s {

    /**
     * The index of the argument that bombed.
     */
    int argindex;

    /**
     * The reason for the problem.
     */
    int error;    
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * General options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_GENERAL[];

/**
 * List options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_LIST[];

/**
 * Extract options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_EXTRACT[];

/**
 * Create options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_CREATE[];

/**
 * Add options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_ADD[];

/**
 * Remove options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_REMOVE[];

/**
 * Diff options.
 */
extern CNWN_PUBLIC const cnwn_CliOption CNWN_ERF_UTIL_OPTIONS_DIFF[];

/**
 * Parse options from command line arguments.
 * @param argc The number of arguments.
 * @param argv The arguments array.
 * @param[out] ret_options Return the options here.
 * @param[out] ret_erf_path Return the ERF path here.
 * @param[out] ret_command Return command here.
 * @param max_errors The maximum number of errors to return.
 * @param[out] ret_errors Return errors here, NULL to omit.
 * @returns The number of returned errors.
 */
extern CNWN_PUBLIC int cnwn_erf_util_parse_options(int argc, char * argv[], cnwn_ERFUtilOptions * ret_options, const char ** ret_erf_path, char * ret_command, int max_errors, cnwn_ERFUtilOptionError * ret_errors);

/**
 * Show ERF info.
 * @param path The path to the ERF file.
 * @param options The options.
 * @returns The number of reported errors.
 */
extern CNWN_PUBLIC int cnwn_erf_util_info(const char * path, const cnwn_ERFUtilOptions * options);

/**
 * List all the entries in an ERF file.
 * @param path The path to the ERF file.
 * @param options The options.
 * @returns The number of reported errors.
 */
extern CNWN_PUBLIC int cnwn_erf_util_list(const char * path, const cnwn_ERFUtilOptions * options);


#ifdef __cplusplus
}
#endif

#endif
