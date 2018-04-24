/**
 * @file file_system.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_FILE_SYSTEM_H
#define CNWN_FILE_SYSTEM_H

#include "cnwn/common.h"
#include "cnwn/path.h"

/**
 * The buffer size to use when doing buffered read/write operations.
 */
#define CNWN_FILE_BUFFER_SIZE 8192

/**
 * File handle.
 *
 * Definition per platform:
 * - Windows: struct cnwn_File_s { HFILE hfile; };
 * - Others: struct cnwn_File_s { int fd; };
 */
typedef struct cnwn_File_s cnwn_File;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Count the files and directories in a directory.
 * @param path Where the directory is, NULL or empty for current working path.
 * @param recurse True to recurse down into sub directories.
 * @returns The number of files and directories in path or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_file_system_count(const char * path, bool recurse);

/**
 * List the files and directories in a directory.
 * @param path Where the directory is, NULL or empty for current working path.
 * @param recurse True to recurse down into sub directories.
 * @returns A newly allocated string array (should be freed with cnwn_strings_free()) or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 */
extern CNWN_PUBLIC char ** cnwn_file_system_ls(const char * path, bool recurse);

/**
 * Create directories.
 * @param path The path to create directories for.
 * @returns The number of created directories or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Already existing directories in the path will simply be ignored.
 */
extern CNWN_PUBLIC int cnwn_file_system_mkdir(const char * path);

/**
 * Remove the file or directory specified by the path.
 * @param path The path of a file or directory to remove.
 * @returns The number of removed files and directories or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will remove a directory and everything inside it recursively.
 */
extern CNWN_PUBLIC int cnwn_file_system_rm(const char * path);

/**
 * Move (or rename) a file or directory.
 * @param path The path of a file or directory to move.
 * @param to_path The destination path (or new name).
 * @returns The number of moved files and directories or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will overwrite any existing files or directories.
 */
extern CNWN_PUBLIC int cnwn_file_system_mv(const char * path, const char * to_path);

/**
 * Copy a file.
 * @param path The path of a file to copy.
 * @param to_path The destination path.
 * @returns The number of copied files or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will overwrite any existing files.
 */
extern CNWN_PUBLIC int cnwn_file_system_cp(const char * path, const char * to_path);

/**
 * Get the size of a file (or directory which will sum up all the file sizes recursively).
 * @param path The path of a file or directory to get the size for.
 * @param recurse True to recurse subdirectories.
 * @returns The size or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Does not count the size for directories themselves.
 */
extern CNWN_PUBLIC int64_t cnwn_file_system_size(const char * path, bool recurse);

/**
 * Check if this the path exists and is a file or directory.
 * @param path The path.
 * @returns A possitive value if the path is a file or directory, zero if not and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_file_system_exists(const char * path);

/**
 * Check if this the path is a file.
 * @param path The path.
 * @returns A possitive value if the path is a file, zero if not and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_file_system_isfile(const char * path);

/**
 * Check if this the path is a directory.
 * @param path The path.
 * @returns A possitive value if the path is a directory, zero if not and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_file_system_isdirectory(const char * path);

/**
 * Open a file.
 * @param path The path to the file.
 * @param mode The file mode to use ("r", "w" and "t". see details). Pass NULL to use default "r".
 * @returns A new file or NULL on error.
 * @see cnwn_get_error() if this function returns NULL.
 *
 * The @p mode can use multiple flags:
 * - "r" is for read mode.
 * - "w" is for write mode.
 * - "t" is for truncate (implicit write mode).
 */
extern CNWN_PUBLIC cnwn_File * cnwn_file_open(const char * path, const char * mode);

/**
 * Close a file.
 * @param f The file to close.
 */
extern CNWN_PUBLIC void cnwn_file_close(cnwn_File * f);

/**
 * Seek file offset.
 * @param f The file to seek.
 * @param offset The seek offset.
 * @returns The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek(cnwn_File * f, int64_t offset);

/**
 * Seek file using delta offset.
 * @param f The file to seek.
 * @param delta_offset The seek delta_offset.
 * @returns The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_delta(cnwn_File * f, int64_t delta_offset);

/**
 * Seek file end.
 * @param f The file to seek.
 * @returns The new offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_seek_end(cnwn_File * f);

/**
 * Return current seek offset.
 * @param f The file to get seek for.
 * @returns The offset or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_get_seek(cnwn_File * f);

/**
 * Read bytes from file.
 * @param f The file to read from.
 * @param size The number of bytes to read.
 * @param[out] ret_buffer Return the read bytes.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read(cnwn_File * f, int64_t size, void * ret_buffer);

/**
 * Write bytes from file.
 * @param f The file to write to.
 * @param size The number of bytes to write.
 * @param buffer The bytes to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write(cnwn_File * f, int64_t size, const void * buffer);

/**
 * Read a fixed number of bytes from file.
 * @param f The file to read from.
 * @param size The fixed number of bytes to read.
 * @param[out] ret_buffer Return the read bytes.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will try a peek for @p size bytes, if there are not enough bytes to read the function will return an error.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read_fixed(cnwn_File * f, int64_t size, void * ret_buffer);

/**
 * Read 64-bit integer from file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read64(cnwn_File * f, int64_t * ret_i);

/**
 * Write 64-bit integer to file.
 * @param f The file to write to.
 * @param i The integer to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write64(cnwn_File * f, int64_t i);

/**
 * Read 32-bit integer from file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read32(cnwn_File * f, int32_t * ret_i);

/**
 * Write 32-bit integer to file.
 * @param f The file to write to.
 * @param i The integer to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write32(cnwn_File * f, int32_t i);

/**
 * Read 16-bit integer from file.
 * @param f The file to read from.
 * @param[out] ret_i Return the integer.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_read16(cnwn_File * f, int16_t * ret_i);

/**
 * Write 16-bit integer to file.
 * @param f The file to write to.
 * @param i The integer to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_write16(cnwn_File * f, int16_t i);

/**
 * Get the current file size (in bytes).
 * @param f The file to get the size from.
 * @returns The file size in bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_size(cnwn_File * f);

/**
 * Copy bytes from one file to another.
 * @param f The file to get the size from.
 * @param size The number of bytes to copy.
 * @param output_f The output file.
 * @returns The number of copied bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_file_copy(cnwn_File * f, int64_t size, cnwn_File * output_f);

#ifdef __cplusplus
}
#endif

#endif
