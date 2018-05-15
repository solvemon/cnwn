/**
 * @file cnwna.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_CNWNA_H
#define CNWN_CNWNA_H

#include "cnwn/options.h"
#include "cnwn/containers.h"
#include "cnwn/regexp.h"

/**
 * @see struct cnwn_CNWNASettings_s
 */
typedef struct cnwn_CNWNASettings_s cnwn_CNWNASettings;

/**
 * Settings for the cnwna executable.
 */
struct cnwn_CNWNASettings_s {

    /**
     * The command.
     */
    char * command;

    /**
     * The path.
     */
    char * path;
    
    /**
     * Not options and not commands (i.e arguments).
     */
    cnwn_StringArray arguments;
    
    /**
     * Show help.
     */
    bool help;

    /**
     * Show version.
     */
    bool version;
    
    /**
     * No output to stdout.
     */
    bool quiet;
    
    /**
     * Recursion depth.
     */
    int depth;

    /**
     * Output path.
     */
    char * output_path;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * General cnwna options.
 */
extern CNWN_PUBLIC const cnwn_Option CNWN_CNWNA_OPTIONS_GENERAL[];

/**
 * Extract command options.
 */
extern CNWN_PUBLIC const cnwn_Option CNWN_CNWNA_OPTIONS_EXTRACT[];

/**
 * Create command options.
 */
extern CNWN_PUBLIC const cnwn_Option CNWN_CNWNA_OPTIONS_CREATE[];

/**
 * Check if the help option is in any of the arguments.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @returns True if there's a help option present.
 * @note This function will ignore any option errors.
 */
extern CNWN_PUBLIC bool cnwn_cnwna_has_help(int argc, char * argv[]);

/**
 * Check if the version option is in any of the arguments.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @returns True if there's a version option present.
 * @note This function will ignore any option errors.
 */
extern CNWN_PUBLIC bool cnwn_cnwna_has_version(int argc, char * argv[]);

/**
 * Parse options into settings.
 * @param settings The settings struct to initialize.
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @returns The number of parsed arguments or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_settings_init(cnwn_CNWNASettings * settings, int argc, char * argv[]);

/**
 * Deinitialize settings.
 * @param settings The settings deinitialize.
 */
extern CNWN_PUBLIC void cnwn_cnwna_settings_deinit(cnwn_CNWNASettings * settings);

/**
 * Print the help string to stdout.
 * @returns The number of printed bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_print_help(void);

/**
 * Print the version string to stdout.
 * @returns The number of printed bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_print_version(void);

/**
 * Execute a command based on settings.
 * @param settings The settings.
 * @returns The number of handled items or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_execute(const cnwn_CNWNASettings * settings);

/**
 * Execute the list command (the command in settings will be ignored).
 * @param path The path to the file to list from.
 * @param quiet True for no stdout output.
 * @param depth The number of levels to recurse listion, a negative value will disable the limit.
 * @param regexps Regular expressions to filter what will be listed, NULL for no filter.
 * @returns The number of listed items or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_execute_list(const char * path, bool quiet, int depth, const cnwn_RegexpArray * regexps);

/**
 * Execute the extract command (the command in settings will be ignored).
 * @param path The path to the file to extract from.
 * @param quiet True for no stdout output.
 * @param depth The number of levels to recurse extraction, a negative value will disable the limit.
 * @param regexps Regular expressions to filter what will be extracted, NULL for no filter.
 * @param output_path The path to output the extracted files and directories.
 * @returns The number of extracted items or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_execute_extract(const char * path, bool quiet, int depth, const cnwn_RegexpArray * regexps, const char * output_path);

/**
 * Execute the create command (the command in settings will be ignored).
 * @param path The path to the file to create.
 * @param quiet True for no stdout output.
 * @param paths The paths to files and directories to add to the file being created.
 * @returns The number of archived items or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_cnwna_execute_create(const char * path, bool quiet, int depth, const cnwn_StringArray * paths);



#ifdef __cplusplus
}
#endif

#endif
