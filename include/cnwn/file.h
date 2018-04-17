/**
 * @file file.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_FILE_H
#define CNWN_FILE_H

#include "cnwn/common.h"

/**
 * The size of the internal buffer when reading/writing.
 */
#define CNWN_READ_WRITE_BUFFER_SIZE 65536

/**
 * The longest allowed path.
 */
#define CNWN_PATH_MAX_SIZE 8192

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
 * @return The file descriptor or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_open_read(const char * path);

/**
 * Open a file for writing.
 * @param path The path of the file to open.
 * @return The file descriptor or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note If the file doesn't exist it will be created and if the file already exists it will be truncated.
 * @note On POSIX file systems the file will have user and group rw flags set.
 */
extern CNWN_PUBLIC int cnwn_open_write(const char * path);

/**
 * Close a file.
 * @param fd The file to close.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC void cnwn_close(int fd);

/**
 * Seek a file descriptor.
 * @param fd The file to seek.
 * @param offset Seek to this offset, a negative value will seek backwards.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_seek(int fd, int64_t offset);

/**
 * Add offset to seek a file descriptor.
 * @param fd The file to seek.
 * @param delta_offset Seek to current offset + this offset.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_seek_delta(int fd, int64_t delta_offset);

/**
 * Seek end of file.
 * @param fd The file to seek.
 * @return The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_seek_end(int fd);

/**
 * Get the current seek offset.
 * @param fd The file to get seek offset for.
 * @return The offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_seek_cur(int fd);

/**
 * Read an unsigned 32-bit integer from a file.
 * @param fd The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_read_uint32(int fd, uint32_t * ret_i);

/**
 * Write an unsigned 32-bit integer to a file.
 * @param fd The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_write_uint32(int fd, uint32_t i);

/**
 * Read an unsigned 16-bit integer from a file.
 * @param fd The file to read from.
 * @param[out] ret_i Return the integer here.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_read_uint16(int fd, uint16_t * ret_i);

/**
 * Write an unsigned 16-bit integer to a file.
 * @param fd The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_write_uint16(int fd, uint16_t i);

/**
 * Read fixed size data.
 * @param fd The file to read from.
 * @param size The size of the fixed size data.
 * @param[out] ret_data Return the data here, pass NULL to omit (bytes will still be read and discarded).
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_read_bytes(int fd, int64_t size, uint8_t * ret_data);

/**
 * Write fixed size data.
 * @param fd The file to write to.
 * @param size The size of the data.
 * @param data The data to write or NULL to write zeroes.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_write_bytes(int fd, int64_t size, const uint8_t * data);

/**
 * Read from the file and write to another file.
 * @param fd The file to read from.
 * @param out_fd The file to write to.
 * @param size The size to read/write.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_copy_bytes(int fd, int out_fd, int64_t size);

/**
 * Check if the path exists.
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
extern CNWN_PUBLIC int cnwn_directory_exists(const char * path);

/**
 * Check if the path exists and is a file.
 * @param path The path to check.
 * @return A positive value if the path exists and is a file, zero if not and a negative value to indicate an error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_file_exists(const char * path);

/**
 * Make sure that all directories specified in the path exist, if not create them.
 * @param path The path to confirm.
 * @return The number of created directories or a negative number on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note On POSIX file systems the new directories will have user and group rwx flags set.
 */
extern CNWN_PUBLIC int cnwn_mkdirs(const char * path);

/**
 * Unescape path.
 * @param path The path to unescape.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_path Return the unescaped path here, pass NULL to omit and get the required length.
 * @return The new length of the unescaped path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_unescape(const char * path, int max_size, char * ret_path);

/**
 * Extract the filename portion from a path.
 * @param path The path to get the filename from.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_filename Return the unescaped path here, pass NULL to omit and get the required length.
 * @return The new length of the filename (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_filename(const char * path, int max_size, char * ret_filename);

/**
 * Extract the directory portion from a path.
 * @param path The path to get the directory from.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_directory Return the unescaped path here, pass NULL to omit and get the required length.
 * @return The new length of the directory (excluding zero terminator).
 * @note The return directory will include the ending path element separator.
 */
extern CNWN_PUBLIC int cnwn_path_directory(const char * path, int max_size, char * ret_directory);

/**
 * Extract the extension portion from a path.
 * @param path The path to get the extension from.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_extension Return the unescaped path here, pass NULL to omit and get the required length.
 * @return The new length of the extension (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_extension(const char * path, int max_size, char * ret_extension);

/**
 * Concatenate multiple paths.
 * @param num_paths The number of paths to concatenate.
 * @param paths The paths to concatenate, NULL or NULL for a path is specific.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_path Return the concatenated path here, NULL to omit.
 * @return The new length of the returned path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_concat(int num_paths, const char * paths[], int max_size, char * ret_path);

/**
 * Append a path to another.
 * @param path The path to append to, NULL is acceptable for no path.
 * @param append The path to append.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_path Return the concatenated path here, NULL to omit.
 * @return The new length of the returned path (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_path_append(const char * path, const char * append, int max_size, char * ret_path);

/**
 * Get filenames in a directory.
 * @param directory The directory to get filenames from.
 * @param A NULL terminated array of regular expression strings, pass NULL to match all.
 * @param extended True if you want to use extended regexps.
 * @return A newly allocated array of string pointers ending with a NULL pointer, must be freed manually or using cnwn_free_strings(), or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC char ** cnwn_directory_contents(const char * directory, const char * regexps[], bool extended);

#ifdef __cplusplus
}
#endif

#endif
