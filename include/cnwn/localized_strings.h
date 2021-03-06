/**
 * @file localized_strings.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_LOCALIZED_STRINGS_H
#define CNWN_LOCALIZED_STRINGS_H

#include "cnwn/common.h"
#include "cnwn/file_system.h"

/**
 * @see struct cnwn_LocalizedString_s
 */
typedef struct cnwn_LocalizedString_s cnwn_LocalizedString;

/**
 * @see cnwn_Array
 */
typedef cnwn_Array cnwn_LocalizedStringArray;

/**
 * Used for localized strings.
 */
struct cnwn_LocalizedString_s {

    /**
     * Language ID.
     */
    int32_t language_id;

    /**
     * The string itself.
     */
    char * text;
};

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Initialize a localized string.
 * @param localized_string The struct to initialize.
 * @param language_id The language ID.
 * @param text The string.
 */
extern CNWN_PUBLIC void cnwn_localized_string_init(cnwn_LocalizedString * localized_string, int language_id, const char * text);

/**
 * Read and initialize the localized string from a file.
 * @param localized_string The localized string.
 * @param f The file, must be at the correct offset.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Any previous content in @p localized_string will be discarded.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_init_from_file(cnwn_LocalizedString * localized_string, cnwn_File * f);

/**
 * Deinitialize a localized string.
 * @param localized_string The localized string to initialize.
 */
extern CNWN_PUBLIC void cnwn_localized_string_deinit(cnwn_LocalizedString * localized_string);

/**
 * Write the localized string to a file.
 * @param localized_string The localized string.
 * @param f The file, must be at the correct offset.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_write(const cnwn_LocalizedString * localized_string, cnwn_File * f);

/**
 * Read all localized strings from a file.
 * @param array The array of localized strings.
 * @param num The number of strings to read.
 * @param f The file, must be at the correct offset.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_array_init_from_file(cnwn_LocalizedStringArray * array, int num, cnwn_File * f);

/**
 * Write the localized strings to a file.
 * @param array The array of localized strings.
 * @param f The file, must be at the correct offset.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_array_write(const cnwn_LocalizedStringArray * array, cnwn_File * f);

/**
 * Append a localized string to the array.
 * @param array The array of localized strings.
 * @param lang_id The lanuage id.
 * @param s The string.
 */
extern CNWN_PUBLIC void cnwn_localized_string_array_append(cnwn_LocalizedStringArray * array, int lang_id, const char * s);

#ifdef __cplusplus
}
#endif

#endif
