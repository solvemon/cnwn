/**
 * @file string.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_STRING_H
#define CNWN_STRING_H

#include "cnwn/common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Check if the characters are the same in case-insensetive ASCII.
 * @param a Char to check for.
 * @param b Char to check for.
 * @returns True if they are considered equal, false if not.
 */
#define CNWN_CHAR_ICASE_EQ(a, b) ((a) >= 'A' && (a) <= 'Z' ? ((a) + 32) == ((b) >= 'A' && (b) <= 'Z' ? ((b) + 32) : (b)) : (a) == ((b) >= 'A' && (b) <= 'Z' ? ((b) + 32) : (b)))

/**
 * Get the string length.
 * @param s The string to get length from.
 * @returns The string length excluding the zero terminator.
 * @note If @p s is NULL zero is returned.
 */
extern CNWN_PUBLIC int cnwn_strlen(const char * s);

/**
 * Get the string length, but limit to @p n chars.
 * @param s The string to get length from.
 * @param n Limit the number of chars to check.
 * @returns The string length excluding the zero terminator.
 * @note If @p s is NULL zero is returned.
 */
extern CNWN_PUBLIC int cnwn_strnlen(const char * s, int n);

/**
 * Compare two strings.
 * @param s The string to compare for.
 * @param o The string to compare with.
 * @returns -1 if @p s < @p o, 1 if @p s > @p o and 0 if @p s == @p o.
 * @note NULL is considered less than an empty string.
 */
extern CNWN_PUBLIC int cnwn_strcmp(const char * s, const char * o);

/**
 * Compare two strings, but limit to @p n chars.
 * @param s The string to compare for.
 * @param o The string to compare with.
 * @param n Limit the number of chars to compare.
 * @returns -1 if @p s < @p o, 1 if @p s > @p o and 0 if @p s == @p o.
 * @note NULL is considered less than an empty string.
 */
extern CNWN_PUBLIC int cnwn_strncmp(const char * s, const char * o, int n);

/**
 * Compare two strings with ASCII case-insensitive comparator.
 * @param s The string to compare for.
 * @param o The string to compare with.
 * @returns -1 if @p s < @p o, 1 if @p s > @p o and 0 if @p s == @p o.
 * @note NULL is considered less than an empty string.
 */
extern CNWN_PUBLIC int cnwn_strcmpi(const char * s, const char * o);

/**
 * Compare two strings with ASCII case-insensitive comparator, but limit to @p n chars.
 * @param s The string to compare for.
 * @param o The string to compare with.
 * @param n Limit the number of chars to compare.
 * @returns -1 if @p s < @p o, 1 if @p s > @p o and 0 if @p s == @p o.
 * @note NULL is considered less than an empty string.
 */
extern CNWN_PUBLIC int cnwn_strncmpi(const char * s, const char * o, int n);

/**
 * Duplicate a string.
 * @param s The string to duplicate.
 * @returns The duplicated string or NULL if @p s is NULL.
 */
extern CNWN_PUBLIC char * cnwn_strdup(const char * s);

/**
 * Duplicate a string, but limit to max @p n chars.
 * @param s The string to duplicate.
 * @param n Limit the number of chars to duplicate.
 * @returns The duplicated string or NULL if @p s is NULL.
 */
extern CNWN_PUBLIC char * cnwn_strndup(const char * s, int n);

/**
 * Copy a string to another.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param s The string to copy from.
 * @param len The length to copy or a negative value to copy all of @p s.
 * @returns The length of the returned string.
 */
extern CNWN_PUBLIC int cnwn_strcpy(char * r, int max_size, const char * s, int len);

/**
 * ASCII upper-case a string.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param s The string to upper.
 * @returns The length of the returned string.
 */
extern CNWN_PUBLIC int cnwn_strupper(char * r, int max_size, const char * s);

/**
 * ASCII lower-case a string.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param s The string to lower.
 * @returns The length of the returned string.
 */
extern CNWN_PUBLIC int cnwn_strlower(char * r, int max_size, const char * s);

/**
 * Remove all ASCII control characters (< 32).
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param s The string to remove control characters from.
 * @returns The length of the returned string.
 */
extern CNWN_PUBLIC int cnwn_strnoctl(char * r, int max_size, const char * s);

/**
 * Concatenate strings from variable arguments.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param sep Insert a separator between the strings.
 * @param args The variable arguments, must be terminated with NULL.
 * @returns The length of the returned string.
 * @note The variable arguments must be terminated with NULL.
 */
extern CNWN_PUBLIC int cnwn_strcat_va(char * r, int max_size, const char * sep, va_list args);

/**
 * Concatenate strings from variable arguments.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param sep Insert a separator between the strings.
 * @returns The length of the returned string.
 * @note The variable arguments must be terminated with NULL.
 */
extern CNWN_PUBLIC int cnwn_strcat(char * r, int max_size, const char * sep, ...);

/**
 * Concatenate strings from a set number of strings.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param sep Insert a separator between the strings.
 * @param num_strings The number of strings to concat.
 * @param strings The strings to concatenate.
 * @returns The length of the returned string.
 * @note The variable arguments must be terminated with NULL.
 */
extern CNWN_PUBLIC int cnwn_strcat2(char * r, int max_size, const char * sep, int num_strings, const char ** strings);

/**
 * Check if the string is blank or not (blank = any ASCII char > 32).
 * @param s The string to check.
 * @returns True if the string is blank, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strisblank(const char * s);

/**
 * Strip all whitespace from the start of the string.
 * @param[out] r Write the return string here, NULL to get required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to strip.
 * @returns The length of the return string.
 */
extern CNWN_PUBLIC int cnwn_strstripstart(char * r, int max_size, const char * s);

/**
 * Strip all whitespace from the end of the string.
 * @param[out] r Write the return string here, NULL to get required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to strip.
 * @returns The length of the return string.
 */
extern CNWN_PUBLIC int cnwn_strstripend(char * r, int max_size, const char * s);

/**
 * Strip all whitespace from the start and end of the string.
 * @param[out] r Write the return string here, NULL to get required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to strip.
 * @returns The length of the return string.
 */
extern CNWN_PUBLIC int cnwn_strstrip(char * r, int max_size, const char * s);

/**
 * Check if a string starts with a substring.
 * @param s The string to check.
 * @param sub The substring to check for.
 * @returns True if the string starts with the substring, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strstartswith(const char * s, const char * sub);

/**
 * Check if a string ends with a substring.
 * @param s The string to check.
 * @param sub The substring to check for.
 * @returns True if the string ends with the substring, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strendswith(const char * s, const char * sub);

/**
 * Find a substring.
 * @param s The string to find substrings in.
 * @param index Where in @p s to start searching for @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The index of where the substring was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strfind(const char * s, int index, const char * sub, const char * esc);

/**
 * Find a substring using case insensitive search (even for @p esc).
 * @param s The string to find substrings in.
 * @param index Where in @p s to start searching for @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The index of where the substring was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strfindi(const char * s, int index, const char * sub, const char * esc);

/**
 * Find a substring, reverse search direction.
 * @param s The string to find substrings in.
 * @param index Where in @p s to start searching for @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The index of where the substring was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strrfind(const char * s, int index, const char * sub, const char * esc);

/**
 * Find a substring using case insensitive search (even for @p esc), reverse search direction.
 * @param s The string to find substrings in.
 * @param index Where in @p s to start searching for @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The index of where the substring was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strrfindi(const char * s, int index, const char * sub, const char * esc);

/**
 * Count occurrences of a substring.
 * @param s The string to count substrings in.
 * @param index Where in @p s to start counting @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The number of occurrences of the substring.
 */
extern CNWN_PUBLIC int cnwn_strcount(const char * s, int index, const char * sub, const char * esc);

/**
 * Count occurrences of a substring using case insensitive search (even for @p esc).
 * @param s The string to count substrings in.
 * @param index Where in @p s to start counting @p sub.
 * @param sub The substring to search for.
 * @param esc Allow escaping of the substring.
 * @returns The number of occurrences of the substring.
 */
extern CNWN_PUBLIC int cnwn_strcounti(const char * s, int index, const char * sub, const char * esc);

/**
 * Split a string into a NULL terminated array of strings.
 * @param s The string to split.
 * @param max_splits The maximum number of splits to make or a negative value to disable the limit.
 * @param sep Split on this separator.
 * @param esc Allow escaping of the separator.
 * @returns The newly allocated and NULL terminated string array (never NULL).
 */
extern CNWN_PUBLIC char ** cnwn_strsplit(const char * s, int max_splits, const char * sep, const char * esc);

/**
 * Split a string into a NULL terminated array of strings using case insensetive search (even for @p esc).
 * @param s The string to split.
 * @param max_splits The maximum number of splits to make or a negative value to disable the limit.
 * @param sep Split on this separator.
 * @param esc Allow escaping of the separator.
 * @returns The newly allocated and NULL terminated string array (never NULL).
 */
extern CNWN_PUBLIC char ** cnwn_strspliti(const char * s, int max_splits, const char * sep, const char * esc);

/**
 * Join a NULL terminated string array into a string.
 * @param r Write result string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string including the zero terminator.
 * @param strings The string to join, must be a NULL terminated array of strings.
 * @param sep Insert a separator between each string.
 * @returns The length of the returned string.
 */
extern CNWN_PUBLIC int cnwn_strjoin(char * r, int max_size, char ** strings, const char * sep);

/**
 * Count the number of strings in a NULL terminated string list.
 * @param strings The string to count, must be a NULL terminated array of strings.
 * @returns The number of strings in the array.
 */
extern CNWN_PUBLIC int cnwn_strings_len(char ** strings);

/**
 * Concatenate two NULL terminated string arrays.
 * @param strings The string array to start with, this array will be reallocated.
 * @param other The other strings to concatenate.
 * @returns The reallocated version of @p strings with @p other concatenated.
 */
extern CNWN_PUBLIC char ** cnwn_strings_cat(char ** strings, char ** other);

/**
 * Free a NULL terminated string array (all strings in it and the array itself).
 * @param strings The string array to free, NULL will do nothing.
 */
extern CNWN_PUBLIC void cnwn_strings_free(char ** strings);

#ifdef __cplusplus
}
#endif

#endif
