/**
 * @file hash.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_HASH_H
#define CNWN_HASH_H

#include "cnwn/common.h"

/**
 * 32-bit hash function.
 * @param data The data to get the hash for.
 * @param length The length of the data to hash.
 * @returns A 32-bit hash.
 * @note Implementations should return zero if @p data is NULL or @p length is 0.
 */
typedef uint32_t (*cnwn_HashFunction32)(const void * data, uint32_t length);


#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get a 32-bit hash from data using murmur3.
 * @param data The data to get the hash for.
 * @param length The length (number of bytes) of the data.
 * @returns A 32-bit hash for the provided data.
 */
extern CNWN_PUBLIC uint32_t cnwn_hash32_murmur3(const void * data, uint32_t length);

/**
 * Get a 32-bit hash from data using CRC32.
 * @param data The data to get the hash for.
 * @param length The length (number of bytes) of the data.
 * @returns A 32-bit hash for the provided data.
 */
extern CNWN_PUBLIC uint32_t cnwn_hash32_crc32(const void * data, uint32_t length);

/**
 * Get a 32-bit hash from data using FNV-1a.
 * @param data The data to get the hash for.
 * @param length The length (number of bytes) of the data.
 * @returns A 32-bit hash for the provided data.
 */
extern CNWN_PUBLIC uint32_t cnwn_hash32_fnv1a(const void * data, uint32_t length);

/**
 * Get a 32-bit hash from data using djb2.
 * @param data The data to get the hash for.
 * @param length The length (number of bytes) of the data.
 * @returns A 32-bit hash for the provided data.
 */
extern CNWN_PUBLIC uint32_t cnwn_hash32_djb2(const void * data, uint32_t length);


#ifdef __cplusplus
}
#endif

#endif
