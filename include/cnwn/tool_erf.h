/**
 * @file tool_erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_TOOL_ERF_H
#define CNWN_TOOL_ERF_H

#include "cnwn/resource.h"
#include "cnwn/options.h"
#include "cnwn/erf.h"
#include "cnwn/regexp.h"

/**
 * Check if we are forcing yes on all questions.
 */
#define CNWN_TOOL_ERF_SETTINGS_FORCE_YES(settings) ((const void *)(settings) != NULL && (settings)->yes && !(settings)->no)

/**
 * Check if we are forcing no on all questions.
 */
#define CNWN_TOOL_ERF_SETTINGS_FORCE_NO(settings) ((const void *)(settings) != NULL && !(settings)->yes && (settings)->no)

/**
 * @see struct cnwn_ToolErfSettings_s
 */
typedef struct cnwn_ToolErfSettings_s cnwn_ToolErfSettings;

/**
 * Settings for the tool.
 */
struct cnwn_ToolErfSettings_s {

    /**
     * Verbose or not.
     */
    bool verbose;

    /**
     * Regular expressions exclude files (instead of includes).
     */
    bool exclude;

    /**
     * Force binary formats.
     */
    bool binary;
    
    /**
     * Assume yes on all questions.
     */
    bool yes;
    
    /**
     * Assume no on all questions.
     */
    bool no;

    /**
     * Temporary file.
     */
    const char * tempfile;
    
    /**
     * Directory.
     */
    const char * directory;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Run the info command.
 * @param settings Some settings.
 * @param path The path to the ERF file.
 * @returns Return the number of reported errors.
 */
extern CNWN_PUBLIC int cnwn_tool_erf_command_info(const cnwn_ToolErfSettings * settings, const char * path);

/**
 * Run the list command.
 * @param settings Some settings.
 * @param path The path to the ERF file.
 * @param argc The argument count.
 * @param argv The arguments.
 * @returns Return the number of reported errors.
 */
extern CNWN_PUBLIC int cnwn_tool_erf_command_list(const cnwn_ToolErfSettings * settings, const char * path, int argc, char * argv[]);

/**
 * Run the extract command.
 * @param settings Some settings.
 * @param path The path to the ERF file.
 * @param argc The argument count.
 * @param argv The arguments.
 * @returns Return the number of reported errors.
 */
extern CNWN_PUBLIC int cnwn_tool_erf_command_extract(const cnwn_ToolErfSettings * settings, const char * path, int argc, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
