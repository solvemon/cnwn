/**
 * @file tool_erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_TOOL_ERF_H
#define CNWN_TOOL_ERF_H

#include "cnwn/resource.h"
#include "cnwn/options.h"
#include "cnwn/erf.h"

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

#ifdef __cplusplus
}
#endif

#endif
