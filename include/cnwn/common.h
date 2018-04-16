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
 * @see struct cnwn_Version_e
 */
typedef struct cnwn_Version_s cnwn_Version;

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
extern CNWN_PUBLIC void cnwn_set_error_va(const char * format, va_list args);

/**
 * Set an error.
 * @param format The error format.
 */
extern CNWN_PUBLIC void cnwn_set_error(const char * format, ...) CNWN_PRINTF(1, 2);

/**
 * Byte swap an unsigned 64-bit integer from little endian to big endian.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 * @note On little endian systems (such as Intel) this function does nothing.
 */
extern CNWN_PUBLIC uint64_t cnwn_swap64(uint64_t i);

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

#ifdef __cplusplus
}
#endif

#endif
