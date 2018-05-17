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
 * Duplicate a string, but limit to max @p n chars.
 * @param s The string to duplicate.
 * @param i The index of where to start the duplication.
 * @param n Limit the number of chars to duplicate or a negative value for the remainder of the string after index @p i.
 * @returns The duplicated string or NULL if @p s is NULL.
 */
extern CNWN_PUBLIC char * cnwn_strndup2(const char * s, int i, int n);

/**
 * Allocate a new string to accomodate format.
 * @param format The format.
 * @param args The variable arguments for the format.
 * @returns The newly allocated and formatted string.
 */
extern CNWN_PUBLIC char * cnwn_strformat_va(const char * format, va_list args);

/**
 * Allocate a new string to accomodate format.
 * @param format The format.
 * @returns The newly allocated and formatted string.
 */
extern CNWN_PUBLIC char * cnwn_strformat(const char * format, ...) CNWN_PRINTF(1, 2);

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
 * Check if a string starts with a substring, case insensetive.
 * @param s The string to check.
 * @param sub The substring to check for.
 * @returns True if the string starts with the substring, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strstartswithi(const char * s, const char * sub);

/**
 * Check if a string ends with a substring, case insensetive.
 * @param s The string to check.
 * @param sub The substring to check for.
 * @returns True if the string ends with the substring, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strendswithi(const char * s, const char * sub);

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
 * Find a char in a string.
 * @param s The string to find the char in.
 * @param index Where in @p s to start searching for @p sub.
 * @param c The char to search for.
 * @param e Allow escaping of the char.
 * @returns The index of where the char was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strfindchar(const char * s, int index, char c, char e);

/**
 * Find a char in a string with case insensetive search.
 * @param s The string to find the char in.
 * @param index Where in @p s to start searching for @p sub.
 * @param c The char to search for.
 * @param e Allow escaping of the char.
 * @returns The index of where the char was found or a negative value if it wasn't found.
 */
extern CNWN_PUBLIC int cnwn_strfindchari(const char * s, int index, char c, char e);

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
 * Escape all occurrences of @p sub that aren't already escaped.
 * @param[out] r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to escape.
 * @param sub The substring to escape.
 * @param esc The escape.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strescape(char * r, int max_size, const char * s, const char * sub, const char * esc);

/**
 * Unescape all occurrences of @p sub that are escaped.
 * @param[out] r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to escape.
 * @param sub The substring to escape.
 * @param esc The escape.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strunescape(char * r, int max_size, const char * s, const char * sub, const char * esc);

/**
 * Remove all control characters from a string (ascii < 32).
 * @param[out] r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to remove controls for.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strnoctl(char * r, int max_size, const char * s);

/**
 * Insert a string into another string.
 * @param r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to insert into.
 * @param index The index of where to insert the string, negative values will wrap from the end of the string.
 * @param format The format of the string to insert.
 * @param args The variable arguments for the format.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strinsert_va(char * r, int max_size, const char * s, int index, const char * format, va_list args);

/**
 * Insert a string into another string.
 * @param r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to insert into.
 * @param index The index of where to insert the string, negative values will wrap from the end of the string.
 * @param format The format of the string to insert.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strinsert(char * r, int max_size, const char * s, int index, const char * format, ...) CNWN_PRINTF(5, 6);

/**
 * Insert a string into another string, realloc and return the new string.
 * @param s The string to insert into, will be reallocated.
 * @param index The index of where to insert the string, negative values will wrap from the end of the string.
 * @param format The format of the string to insert.
 * @param args The variable arguments for the format.
 * @returns The reallocated version of @p s with the formatted string inserted.
 */
extern CNWN_PUBLIC char * cnwn_strinsert_realloc_va(char * s, int index, const char * format, va_list args);

/**
 * Insert a string into another string, realloc and return the new string.
 * @param s The string to insert into, will be reallocated.
 * @param index The index of where to insert the string, negative values will wrap from the end of the string.
 * @param format The format of the string to insert.
 * @returns The reallocated version of @p s with the formatted string inserted.
 */
extern CNWN_PUBLIC char * cnwn_strinsert_realloc(char * s, int index, const char * format, ...) CNWN_PRINTF(3, 4);

/**
 * Append a string to another string.
 * @param r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to append into.
 * @param format The format of the string to append.
 * @param args The variable arguments for the format.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strappend_va(char * r, int max_size, const char * s, const char * format, va_list args);

/**
 * Append a string to another string.
 * @param r Write the return string here, pass NULL to get the required length.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param s The string to append into.
 * @param format The format of the string to append.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_strappend(char * r, int max_size, const char * s, const char * format, ...) CNWN_PRINTF(4, 5);

/**
 * Append a string to another string, realloc and return the new string.
 * @param s The string to append into, will be reallocated.
 * @param index The index of where to append the string, negative values will wrap from the end of the string.
 * @param format The format of the string to append.
 * @param args The variable arguments for the format.
 * @returns The reallocated version of @p s with the formatted string appended.
 */
extern CNWN_PUBLIC char * cnwn_strappend_realloc_va(char * s, const char * format, va_list args);

/**
 * Append a string to another string, realloc and return the new string.
 * @param s The string to append into, will be reallocated.
 * @param index The index of where to append the string, negative values will wrap from the end of the string.
 * @param format The format of the string to append.
 * @returns The reallocated version of @p s with the formatted string appended.
 */
extern CNWN_PUBLIC char * cnwn_strappend_realloc(char * s, const char * format, ...) CNWN_PRINTF(2, 3);

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

/**
 * Get the integer base for a string based on it's prefix.
 * @param s The string to get a base from.
 * @returns The integer base (2, 8, 10 or 16) or zero if no valid prefix and no valid number was found.
 */
extern CNWN_PUBLIC int cnwn_strintbase(const char * s);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strshort(const char * s, int base, short * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC short cnwn_strtoshort(const char * s, int base, short errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strushort(const char * s, int base, unsigned short * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC unsigned short cnwn_strtoushort(const char * s, int base, unsigned short errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strint(const char * s, int base, int * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC int cnwn_strtoint(const char * s, int base, int errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_struint(const char * s, int base, unsigned int * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC unsigned int cnwn_strtouint(const char * s, int base, unsigned int errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strlong(const char * s, int base, long * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC long cnwn_strtolong(const char * s, int base, long errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strulong(const char * s, int base, unsigned long * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC unsigned long cnwn_strtoulong(const char * s, int base, unsigned long errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strlonglong(const char * s, int base, long long * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC long long cnwn_strtolonglong(const char * s, int base, long long errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strulonglong(const char * s, int base, unsigned long long * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC unsigned long long cnwn_strtoulonglong(const char * s, int base, unsigned long long errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strint8(const char * s, int base, int8_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC int8_t cnwn_strtoint8(const char * s, int base, int8_t errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_struint8(const char * s, int base, uint8_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC uint8_t cnwn_strtouint8(const char * s, int base, uint8_t errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strint16(const char * s, int base, int16_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC int16_t cnwn_strtoint16(const char * s, int base, int16_t errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_struint16(const char * s, int base, uint16_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC uint16_t cnwn_strtouint16(const char * s, int base, uint16_t errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strint32(const char * s, int base, int32_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC int32_t cnwn_strtoint32(const char * s, int base, int32_t errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_struint32(const char * s, int base, uint32_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC uint32_t cnwn_strtouint32(const char * s, int base, uint32_t errvalue);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strint64(const char * s, int base, int64_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC int64_t cnwn_strtoint64(const char * s, int base, int64_t errvalue);

/**
 * Parse a string into an unsignedinteger.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param[out] ret_value Return the integer value here, pass NULL to just check if it's a valid integer string.
 * @returns True if the string could be parsed to an integer, false if not.
 */
extern CNWN_PUBLIC bool cnwn_struint64(const char * s, int base, uint64_t * ret_value);

/**
 * Parse a string into an integer.
 * @param s The string to parse.
 * @param base Chosen base, zero or negative to auto-detect from possible prefix of @p s.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The integer value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC uint64_t cnwn_strtouint64(const char * s, int base, uint64_t errvalue);

/**
 * Parse a string into a double.
 * @param s The string to parse.
 * @param[out] ret_value Return the double value here, pass NULL to just check if it's a valid floating point string.
 * @returns True if the string could be parsed to a double, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strdouble(const char * s, double * ret_value);

/**
 * Parse a string into a double.
 * @param s The string to parse.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The floating point value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC double cnwn_strtodouble(const char * s, double errvalue);

/**
 * Parse a string into a float.
 * @param s The string to parse.
 * @param[out] ret_value Return the float value here, pass NULL to just check if it's a valid floating point string.
 * @returns True if the string could be parsed to a float, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strfloat(const char * s, float * ret_value);

/**
 * Parse a string into a float.
 * @param s The string to parse.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The floating point value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC float cnwn_strtofloat(const char * s, float errvalue);

/**
 * Parse a string into a bool.
 * @param s The string to parse.
 * @param[out] ret_value Return the bool value here, pass NULL to just check if it's a valid bool string.
 * @returns True if the string could be parsed to a bool, false if not.
 */
extern CNWN_PUBLIC bool cnwn_strbool(const char * s, bool * ret_value);

/**
 * Parse a string into a bool.
 * @param s The string to parse.
 * @param errvalue The value to return if @p s could not be parsed.
 * @returns The bool value or @p errvalue if @p s could not be parsed.
 */
extern CNWN_PUBLIC bool cnwn_strtobool(const char * s, bool errvalue);

#ifdef __cplusplus
}
#endif

#endif
