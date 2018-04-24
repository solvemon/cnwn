/**
 * @file endian.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ENDIAN_H
#define CNWN_ENDIAN_H

#include "cnwn/common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Swap endian of a 64-bit integer.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int64_t cnwn_endian_swap64(int64_t i);

/**
 * Swap endian of a 32-bit integer.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int32_t cnwn_endian_swap32(int32_t i);

/**
 * Swap endian of a 16-bit integer.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int16_t cnwn_endian_swap16(int16_t i);

/**
 * Ensure that a 64-bit integer is big endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On big endian hosts this does nothing.
 */
extern CNWN_PUBLIC int64_t cnwn_endian_big64(int64_t i);

/**
 * Ensure that a 32-bit integer is big endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On big endian hosts this does nothing.
 */
extern CNWN_PUBLIC int32_t cnwn_endian_big32(int32_t i);

/**
 * Ensure that a 16-bit integer is big endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On big endian hosts this does nothing.
 */
extern CNWN_PUBLIC int16_t cnwn_endian_big16(int16_t i);

/**
 * Ensure that a 64-bit integer is little endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On little endian hosts this does nothing.
 */
extern CNWN_PUBLIC int64_t cnwn_endian_little64(int64_t i);

/**
 * Ensure that a 32-bit integer is little endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On little endian hosts this does nothing.
 */
extern CNWN_PUBLIC int32_t cnwn_endian_little32(int32_t i);

/**
 * Ensure that a 16-bit integer is little endian.
 * @param i The integer to possibly byte swap.
 * @return The possibly swapped integer.
 * @note On little endian hosts this does nothing.
 */
extern CNWN_PUBLIC int16_t cnwn_endian_little16(int16_t i);


#ifdef __cplusplus
}
#endif

#endif
