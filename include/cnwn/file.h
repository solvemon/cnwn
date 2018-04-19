/**
 * @file file.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_FILE_H
#define CNWN_FILE_H

#include "cnwn/common.h"
#include "cnwn/regulexp.h"

/**
 * The size of the internal buffer when reading/writing.
 */
#define CNWN_READ_WRITE_BUFFER_SIZE 65536

/**
 * The longest allowed path.
 */
#define CNWN_PATH_MAX_SIZE 8192

/**
 * Abstract representation of a file.
 * @note It's actual contents may vary depending on platform, see details for more info.
 *
 * POSIX struct:
 * struct cnwn_File_s { int fd; };
 */
typedef struct cnwn_File_s cnwn_File;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The character used to separate path elements.
 **/
extern CNWN_PUBLIC const char CNWN_PATH_SEPARATOR;

/**
 * The character used to escape path characters.
 **/
extern CNWN_PUBLIC const char CNWN_PATH_ESCAPE;

/**
 * Open a file for reading.
 * @param path The path of the file to open.
 * @return The file or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC cnwn_File * cnwn_file_open_r(const char * path);

/**
 * Open a file for writing.
 * @param path The path of the file to open.
 * @return The file or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 * @note If the file doesn't exist it will be created and if the file already exists it will be truncated.
 * @note On POSIX file systems the file will have user and group rw flags set.
 */
extern CNWN_PUBLIC cnwn_File * cnwn_file_open_w(const char * path);

/**
 * Open a file for reading and writing.
 * @param path The path of the file to open.
 * @return The file or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 * @note If the file doesn't exist it will be created and if the file already exists it will be truncated.
 * @note On POSIX file systems the file will have user and group rw flags set.
 */
extern CNWN_PUBLIC cnwn_File * cnwn_file_open_rw(const char * path);

/**
 * Open a file for reading and writing, DON'T truncate.
 * @param path The path of the file to open.
 * @return The file or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 * @note If the file doesn't exist it will be created and if the file already exists it will be truncated.
 * @note On POSIX file systems the file will have user and group rw flags set.
 */
extern CNWN_PUBLIC cnwn_File * cnwn_file_open_rw_notrunc(const char * path);

/**
 * Close a file.
 * @param f The file to close.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC void cnwn_file_close(cnwn_File * f);

/**
 * Seek a file descriptor.
 * @param f The file to seek.
 * @param offset Seek to this offset, a negative value will seek backwards.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek(cnwn_File * f, int64_t offset);

/**
 * Add offset to seek a file descriptor.
 * @param f The file to seek.
 * @param delta_offset Seek to current offset + @p delta_offset, negative values are acceptable.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_delta(cnwn_File * f, int64_t delta_offset);

/**
 * Seek start of file.
 * @param f The file to seek.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_start(cnwn_File * f);

/**
 * Seek end of file.
 * @param f The file to seek.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_end(cnwn_File * f);

/**
 * Get the current seek offset.
 * @param f The file to seek.
 * @return The offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_cur(cnwn_File * f);

/**
 * Read data from a file.
 * @param f The file to read from.
 * @param size Number of bytes to read.
 * @param[out] ret_data Return the data here, pass NULL to omit (bytes will still be read and discarded).
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read(cnwn_File * f, int64_t size, uint8_t * ret_data);

/**
 * Write data to a file.
 * @param f The file to write to.
 * @param size Number of bytes to write.
 * @param data The data to write or NULL to write zeroes.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write(cnwn_File * f, int64_t size, const uint8_t * data);

/**
 * Read from the file and write to another file (i.e "copy").
 * @param f The file to read from.
 * @param output_f The file to write to.
 * @param size The number of bytes to copy.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_copy_bytes(cnwn_File * f, cnwn_File * output_f, int64_t size);

/**
 * Read an unsigned 64-bit integer from a file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read_uint64(cnwn_File * f, uint64_t * ret_i);

/**
 * Write an unsigned 64-bit integer to a file.
 * @param f The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write_uint64(cnwn_File * f, uint64_t i);

/**
 * Read an unsigned 32-bit integer from a file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read_uint32(cnwn_File * f, uint32_t * ret_i);

/**
 * Write an unsigned 32-bit integer to a file.
 * @param f The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write_uint32(cnwn_File * f, uint32_t i);

/**
 * Read an unsigned 16-bit integer from a file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read_uint16(cnwn_File * f, uint16_t * ret_i);

/**
 * Write an unsigned 16-bit integer to a file.
 * @param f The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write_uint16(cnwn_File * f, uint16_t i);

/**
 * Read an unsigned 8-bit integer from a file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read_uint8(cnwn_File * f, uint8_t * ret_i);

/**
 * Write an unsigned 8-bit integer to a file.
 * @param f The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write_uint8(cnwn_File * f, uint8_t i);

/**
 * Unescape path.
 * @param[out] s Return the unescaped path here, pass NULL to omit and get the required length.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param path The path to unescape.
 * @return The new length of the unescaped path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_unescape(char * s, int max_size, const char * path);

/**
 * Concatenate multiple paths.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @return The new length of the returned path (excluding zero terminator).
 * @note The variable arguments MUST be terminated with a NULL pointer sentinel.
 */
extern CNWN_PUBLIC int cnwn_path_concat(char * s, int max_size, ...);

/**
 * Split a path into it's elements.
 * @param path The path to split.
 * @param max_splits The maximum number of splits, negative values will disable the limit.
 * @return A newly allocated array of string pointers (will never be NULL) that must be freed manually or using cnwn_free_strings().
 */
extern CNWN_PUBLIC char ** cnwn_path_split(const char * path, int max_splits);

/**
 * Join a string list into a path.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param strings The string to join as a path.
 * @return The new length of the returned path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_join(char * s, int max_size, char ** strings);

/**
 * Get the directory part of a path.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param path The path to get the directory part from.
 * @return The new length of the returned string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_dirpart(char * s, int max_size, const char * path);

/**
 * Get the base (filename + extension) part of a path.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param path The path to get the base part from.
 * @return The new length of the returned string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_basepart(char * s, int max_size, const char * path);

/**
 * Get the filename part of a path.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param path The path to get the filename part from.
 * @return The new length of the returned string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_filepart(char * s, int max_size, const char * path);

/**
 * Get the extension part of a path.
 * @param[out] s Return the concatenated path here, NULL to get the required size.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param path The path to get the extension part from.
 * @return The new length of the returned string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_extpart(char * s, int max_size, const char * path);

/**
 * Check if the path exists and is a file or directory.
 * @param path The path to check.
 * @return A positive value if the path exists, zero if not and a negative value to indicate an error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_path_exists(const char * path);

/**
 * Check if the path exists and is a directory.
 * @param path The path to check.
 * @return A positive value if the path exists and is a directory, zero if not and a negative value to indicate an error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_path_isdir(const char * path);

/**
 * Check if the path exists and is a file.
 * @param path The path to check.
 * @return A positive value if the path exists and is a file, zero if not and a negative value to indicate an error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_path_isfile(const char * path);

/**
 * Make sure that all directories specified in the path exist, if not create them.
 * @param path The path to confirm.
 * @return The number of created directories or a negative number on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note On POSIX file systems the new directories will have user and group rwx flags set.
 * @note Already existing directories will just be ignored.
 */
extern CNWN_PUBLIC int cnwn_mkdirs(const char * path);

/**
 * Get filenames in a directory.
 * @param path A path to the directory to get filenames from.
 * @param full True to use the full path for each file (path + filename), false to just use the filename.
 * @param regexps A NULL terminated array of regular expression strings, pass NULL to match all.
 * @param extended True if you want to use extended regexps.
 * @return A newly allocated array of string pointers ending with a NULL pointer, must be freed manually or using cnwn_free_strings(), or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC char ** cnwn_listdir(const char * path, bool full, const char * regexps[], bool extended);

/**
 * Get the size of a file at specified path.
 * @param path The path of the file to get the size for.
 * @returns The size (in bytes) or a negative value to indicate an error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_path_filesize(const char * path);

#ifdef __cplusplus
}
#endif

#endif
