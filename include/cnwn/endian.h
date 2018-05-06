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
 * Swap endian of a 64-bit integer from little to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 * @note
 */
extern CNWN_PUBLIC int64_t cnwn_endian_ltoh64(int64_t i);

/**
 * Swap endian of a 32-bit integer from little to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int32_t cnwn_endian_ltoh32(int32_t i);

/**
 * Swap endian of a 16-bit integer from little to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int16_t cnwn_endian_ltoh16(int16_t i);

/**
 * Swap endian of a 64-bit integer from big to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 * @note
 */
extern CNWN_PUBLIC int64_t cnwn_endian_btoh64(int64_t i);

/**
 * Swap endian of a 32-bit integer from big to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int32_t cnwn_endian_btoh32(int32_t i);

/**
 * Swap endian of a 16-bit integer from big to whatever the host is using.
 * @param i The integer to byte swap.
 * @return The swapped integer.
 */
extern CNWN_PUBLIC int16_t cnwn_endian_btoh16(int16_t i);


#ifdef __cplusplus
}
#endif

#endif
