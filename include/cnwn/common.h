/**
 * @file common.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_COMMON_H
#define CNWN_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <regex.h>

/**
 * @cond
 */
#ifdef BUILD_API
#if defined(_WIN32) || defined(_WIN64)
#ifndef CNWN_PUBLIC
#define CNWN_PUBLIC __attribute__ ((dllexport))
#endif
#ifndef CNWN_PRIVATE
#define CNWN_PRIVATE
#endif
#else
#ifndef CNWN_PUBLIC
#define CNWN_PUBLIC __attribute__ ((visibility ("default")))
#endif
#ifndef CNWN_PRIVATE
#define CNWN_PRIVATE __attribute__ ((visibility ("hidden")))
#endif
#endif
#else
#if defined(_WIN32) || defined(_WIN64)
#ifndef CNWN_PUBLIC
#define CNWN_PUBLIC __attribute__ ((dllimport))
#endif
#ifndef CNWN_PRIVATE
#define CNWN_PRIVATE
#endif
#else
#ifndef CNWN_PUBLIC
#define CNWN_PUBLIC
#endif
#ifndef CNWN_PRIVATE
#define CNWN_PRIVATE
#endif
#endif
#endif

#ifndef CNWN_FORCE_INLINE
#define CNWN_FORCE_INLINE __attribute__((always_inline)) inline
#endif

#ifndef CNWN_PRINTF
#ifdef __GNUC__
#define CNWN_PRINTF(string_index_, first_to_check_) __attribute__ ((format (printf, string_index_, first_to_check_)))
#else
#define CNWN_PRINTF(string_index_, first_to_check_)
#endif
#endif

#ifndef CNWN_MIN
#define CNWN_MIN(a_, b_) ((a_) < (b_) ? (a_) : (b_))
#endif
#ifndef CNWN_MAX
#define CNWN_MAX(a_, b_) ((a_) > (b_) ? (a_) : (b_))
#endif
#ifndef CNWN_MINMAX
#define CNWN_MINMAX(x_, a_, b_) ((a_) < (b_) ? CNWN_MIN(b_, CNWN_MAX(x_, a_)) : CNWN_MIN(a_, CNWN_MAX(x_, b_)))
#endif

/**
 * @endcond
 */

/**
 * The size of the internal buffer when reading/writing.
 */
#define CNWN_READ_WRITE_BUFFER_SIZE 8192

/**
 * The maximum size of an ERF entry key (including zero terminator).
 * @note The largest key allowed for an ERF entry is 16 (V1.0) and 32 (V1.1) bytes.
 */
#define CNWN_ERF_ENTRY_KEY_MAX_SIZE 33

/**
 * Check whether or not an option is a short option (single dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_IS_SHORT(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] != '-' && (arg)[1] != 0 && (arg)[2] == 0)

/**
 * Check whether or not an option is a long option (double dash).
 * @param arg The argument to check.
 **/
#define CNWN_CLI_OPTION_IS_LONG(arg) ((void *)(arg) != NULL && (arg)[0] == '-' && (arg)[1] == '-' && (arg)[2] != 0)

/**
 * The longest allowed path.
 */
#define CNWN_PATH_MAX_SIZE 8192

/**
 * The option is recognized as a normal argument.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_ARG -1

/**
 * An invalid option was specified.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_INVALID -2

/**
 * A valid option expected an argument, but none was found.
 * @note Returned by cnwn_parse_cli_option().
 */
#define CNWN_CLI_OPTION_INCOMPLETE -3

/**
 * @see struct cnwn_CliOption_s
 */
typedef struct cnwn_CliOption_s cnwn_CliOption;

/**
 * @see struct cnwn_Version_e
 */
typedef struct cnwn_Version_s cnwn_Version;

/**
 * Cli option definition.
 */
struct cnwn_CliOption_s {

    /**
     * Option key.
     */
    const char * key;
    
    /**
     * True if the option requires an argument.
     */
    bool requires_argument;

    /**
     * A help string.
     */
    const char * help;
};

/***
 * Keep track of major/minor version.
 */
struct cnwn_Version_s {

    /**
     * Major.
     */
    int major;

    /**
     * Minor.
     */
    int minor;
};

#ifdef __cplusplus
extern "C" {
#endif

////////////////////////////////////////////////////////////////
//
//
// Constants.
//
//
////////////////////////////////////////////////////////////////

/**
 * The character used to separate path elements.
 **/
extern CNWN_PUBLIC const char CNWN_PATH_SEPARATOR;

/**
 * The character used to escape path characters.
 **/
extern CNWN_PUBLIC const char CNWN_PATH_ESCAPE;

////////////////////////////////////////////////////////////////
//
//
// Common functions (IO and what not).
//
//
////////////////////////////////////////////////////////////////

/**
 * Parse some cli options.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param arg The argument.
 * @param next The argument following @p arg or NULL if there are no more arguments.
 * @param[out] ret_arg Return an argument, either to an option (if the function returns >= 0, can be NULL if the option has no argument) or as a normal non-option argument if the function returns @ref CNWN_CLI_OPTION_ARG.
 * @returns A zero or possitive option index of a valid option or a negative value for non-options, see details for result codes.
 * @note To check whether you need to skip one argument in argv (for long options that return non-NULL @p ret_arg) use the CNWN_CLI_OPTION_IS_SHORT() macro.
 *
 * Result codes:
 * - >= 0 is a valid option index.
 * CNWN_CLI_OPTION_ARG to indicate a normal argument.
 * CNWN_CLI_OPTION_INVALID to indicate an invalid option.
 * CNWN_CLI_OPTION_INCOMPLETE to indicate a valid option with missing argument.
 */
extern CNWN_PUBLIC int cnwn_parse_cli_option(const cnwn_CliOption * options, const char * arg, const char * next, const char ** ret_arg);

/**
 * Print help text for options.
 * @param options A list of available options, this list must be terminated by a zeroed struct sentinel.
 * @param prefix Prefix every printed line with this string (NULL is OK).
 */
extern CNWN_PUBLIC void cnwn_print_cli_options(const cnwn_CliOption * options, const char * prefix);

/**
 * Get the last error message.
 * @returns A pointer to the last error message, never NULL.
 */
extern CNWN_PUBLIC const char * cnwn_get_error(void);

/**
 * Set an error.
 * @param format The error format.
 * @param args The variable arguments.
 */
extern CNWN_PRIVATE void cnwn_set_error_va(const char * format, va_list args);

/**
 * Set an error.
 * @param format The error format.
 */
extern CNWN_PRIVATE void cnwn_set_error(const char * format, ...) CNWN_PRINTF(1, 2);

/**
 * Byte swap an unsigned 32-bit integer from little endian to big endian.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 * @note On little endian systems (such as Intel) this function does nothing.
 */
extern CNWN_PUBLIC uint32_t cnwn_swap32(uint32_t i);

/**
 * Byte swap an unsigned 16-bit integer from little endian to big endian.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 * @note On little endian systems (such as Intel) this function does nothing.
 */
extern CNWN_PUBLIC uint16_t cnwn_swap16(uint16_t i);

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
 * Clean-up a string as required for resource names.
 * @param s The string to clean.
 * @param max_size The max size for the returned string (including zero terminator).
 * @param[out] ret_s Return the cleaned string here, NULL or the same as @p s is acceptable.
 * @return The new length of the string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_clean_string(const char * s, int max_size, char * ret_s);

#ifdef __cplusplus
}
#endif

#endif
