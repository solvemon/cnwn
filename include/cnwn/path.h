/**
 * @file path.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_PATH_H
#define CNWN_PATH_H

#include "cnwn/common.h"
#include "cnwn/string.h"

/**
 * The maximum size for a path.
 */
#define CNWN_PATH_MAX_SIZE 8192

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The path element seperator.
 */
extern CNWN_PUBLIC const char * CNWN_PATH_SEPARATOR;

/**
 * The path element seperator escape.
 */
extern CNWN_PUBLIC const char * CNWN_PATH_ESCAPE;

/**
 * Depends on the platform.
 */
extern CNWN_PUBLIC const bool CNWN_PATH_CASE_INSENSETIVE;

/**
 * Clean a path (remove empty path elements and any trailing path separators).
 * @param[out] r Return the cleaned path here, NULL to get the new length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param path The path to clean.
 * @returns The length of the cleaned path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_clean(char * r, int max_size, const char * path);

/**
 * Check if the path describes either a file or directory.
 * @param path The path to check.
 * @returns True if valid, false if not.
 *
 * - "." and ".." will return false.
 * - "C:" (where C could be any drive) will return false.
 * - NULL or an empty string will return false.
 */
extern CNWN_PUBLIC bool cnwn_path_isfiledirectory(const char * path);

/**
 * Get the directory part of a path.
 * @param[out] r Return the directory part here, NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param path The path to get the directory part from.
 * @returns The length of the directory part (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_directorypart(char * r, int max_size, const char * path);

/**
 * Get the base part of a path.
 * @param[out] r Return the base part here, NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param path The path to get the base part from.
 * @returns The length of the base part (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_basepart(char * r, int max_size, const char * path);

/**
 * Get the filename part excluding the filename extensions.
 * @param[out] r Return the filename part here, NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param path The path to get the filename part from.
 * @returns The length of the filename part (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_filenamepart(char * r, int max_size, const char * path);

/**
 * Get the filename extension part (everything after the first filename dot).
 * @param[out] r Return the filename extension part here, NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param path The path to get the filename part from.
 * @returns The length of the filename extension part (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_extensionpart(char * r, int max_size, const char * path);

/**
 * Split a path into a NULL terminated array of strings.
 * @param path The path to split.
 * @param max_splits The maximum number of splits to make or a negative value to disable the limit.
 * @returns The newly allocated and NULL terminated string array (never NULL).
 */
extern CNWN_PUBLIC char ** cnwn_path_split(const char * path, int max_splits);

/**
 * Join a NULL terminated string array into a path.
 * @param r Write result path here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param strings The string to join, must be a NULL terminated array of strings.
 * @returns The length of the returned path.
 */
extern CNWN_PUBLIC int cnwn_path_join(char * r, int max_size, char ** strings);

#ifdef __cplusplus
}
#endif

#endif
