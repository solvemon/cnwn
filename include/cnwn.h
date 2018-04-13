/**
 * @file cnwn.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_H
#define CNWN_H

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
 * Get the extension for a resource type.
 * @param ct The resource type enum.
 * @returns A pointer to a string describing the enumeration extension or an empty string if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_EXTENSION(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_RESOURCE_INFOS[(ct)].extension : "")

/**
 * Get human readable version of the ERF type enumeration.
 * @param ct The resource type enum.
 * @returns A pointer to a string describing the enumeration name or an empty string if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_NAME(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_RESOURCE_INFOS[(ct)].name : "")

/**
 * Get the ERF type for a resource type.
 * @param ct The resource type enum.
 * @returns An ERF type or -1 if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_ERF_TYPE(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_RESOURCE_INFOS[(ct)].erf_type : -1)

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
 * The longest allowed path.
 */
#define CNWN_PATH_MAX_SIZE 8192

/**
 * Describes an ERF entry type.
 */
enum cnwn_ResourceType_e {

    /**
     * Unknown resource type.
     */
    CNWN_RESOURCE_TYPE_NONE = 0,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RES,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BMP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MVE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TGA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WAV,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_INI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MP3,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MPG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLH,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TEX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDL,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_THG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FNT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LUA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SLT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NSS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NCS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MOD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ARE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SET,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFO,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WOK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_2DA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TLK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTI,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DLG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ITP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DDS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTR,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GFF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FAC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTE,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DFT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GUI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CSS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CCS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DWK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTG,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JRL,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SAV,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTW,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_4PC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SSF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_HAK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NWM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NDB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTM,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BAK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_OSC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_USC,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRN,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTR,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UEN,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ULT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SEF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CAM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UPE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ROS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RST,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ZIP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WMP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BBX,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WLK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_XML,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SCC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SPT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GR2,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JPG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IDS,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ERF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_KEY,
      
    /**
     * Max enum.
     */
    CNWN_MAX_RESOURCE_TYPE
};

/**
 * @see enum cnwn_ResourceType_e
 */
typedef enum cnwn_ResourceType_e cnwn_ResourceType;

/**
 * @see struct cnwn_ResourceInfo_s
 */
typedef struct cnwn_ResourceInfo_s cnwn_ResourceInfo;

/**
 * @see struct cnwn_Version_e
 */
typedef struct cnwn_Version_s cnwn_Version;

/**
 * @see struct cnwn_ERFHeader_e
 */
typedef struct cnwn_ERFHeader_s cnwn_ERFHeader;

/**
 * @see struct cnwn_ERFEntry_e
 */
typedef struct cnwn_ERFEntry_s cnwn_ERFEntry;

/**
 * Info about a resource.
 */
struct cnwn_ResourceInfo_s {

    /**
     * The extension.
     */
    const char * extension;
    
    /**
     * A human readable name.
     */
    const char * name;

    /**
     * The internal ERF type.
     */
    int erf_type;
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

/**
 * ERF header.
 */
struct cnwn_ERFHeader_s {

    /**
     * The type of resource the ERF is.
     */
    cnwn_ResourceType type;

    /**
     * The type string as it was read from a file.
     */
    char type_str[5];
    
    /**
     * The version.
     */
    cnwn_Version version;

    /**
     * The version string as it was read from a file.
     */
    char version_str[5];
    
    /**
     * The number of localized strings.
     */
    uint32_t num_localized_strings;

    /**
     * The offset of the localized strings.
     */
    uint32_t localized_strings_offset;
    
    /**
     * The size of the localized strings.
     */
    uint32_t localized_strings_size;
    
    /**
     * The number of entries.
     */
    uint32_t num_entries;

    /**
     * The offset of the entry keys.
     */
    uint32_t keys_offset;
    
    /**
     * The offset for the entry resources.
     */
    uint32_t resources_offset;
};

/**
 * ERF entry.
 */
struct cnwn_ERFEntry_s {

    /**
     * The resource type the entry is.
     */
    cnwn_ResourceType type;

    /**
     * The key (name) of the resource.
     */
    char key[CNWN_ERF_ENTRY_KEY_MAX_SIZE];

    /**
     * The offset to the key.
     */
    uint32_t key_offset;
    
    /**
     * The offset to the resource.
     */
    uint32_t resource_offset;

    /**
     * The ID of the resource.
     */
    uint32_t resource_id;

    /**
     * The ERF type of the resource.
     */
    uint16_t erf_type;
    
    /**
     * Some unused bytes.
     */
    uint16_t unused;

    /**
     * The size of the resource.
     */
    uint32_t resource_size;
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

/**
 * Info about each resource type.
 * @note Defined in src/resources.c
 */
extern CNWN_PUBLIC const cnwn_ResourceInfo CNWN_RESOURCE_INFOS[CNWN_MAX_RESOURCE_TYPE];

////////////////////////////////////////////////////////////////
//
//
// Common functions (IO and what not).
//
//
////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////
//
//
// Resource functions.
//
//
////////////////////////////////////////////////////////////////

/**
 * Get a resource type from an ERF resource type.
 * @param erf_type The ERF type.
 * @returns The resource type or CNWN_RESOURCE_TYPE_NONE if @p erf_type is invalid.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_erf_type(int erf_type);

/**
 * Get a resource type from an ERF resource type.
 * @param extension The filename extension.
 * @returns The resource type or CNWN_RESOURCE_TYPE_NONE if @p extension is invalid.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_extension(const char * extension);

/**
 * Convert a resource type and entry key to a filename.
 * @param resource_type The resource type (will be used to determine the filename extension).
 * @param key The entry key (max 16 characters for V1.0 and max 32 characters for V1.1, excluding zero terminator), must not be NULL or empty.
 * @param max_size The maximum size of the return string (including zero terminator).
 * @param ret_filename Return the filename here, NULL to omit.
 * @return The length (or required length if @p ret_filename is NULL) of the filename.
 */
extern CNWN_PUBLIC int cnwn_resource_to_filename(cnwn_ResourceType resource_type, const char * key, int max_size, char * ret_filename);

/**
 * Convert a filename into a resource type and entry key.
 * @param filename The filename to parse, only the (file) base part of the path will be used (directories will be removed).
 * @param[out] ret_resource_type The resource type (determined by the filename's extension).
 * @param max_size The maximum size of the return key (including zero terminator), should be at least @p ref CNWN_ERF_ENTRY_KEY_MAX_SIZE.
 * @param[out] ret_key The key, NULL to omit.
 * @return The length (or required length if @p ret_key is NULL) of the key.
 */
extern CNWN_PUBLIC int cnwn_resource_from_filename(const char * filename, cnwn_ResourceType * ret_resource_type, int max_size, char * ret_key);

////////////////////////////////////////////////////////////////
//
//
// ERF functions.
//
//
////////////////////////////////////////////////////////////////

/**
 * Read the header and entries from an ERF file.
 * @param fd The file to read from.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Return entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read(int fd, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Write the header and entries to an ERF file.
 * @param fd The file to write to.
 * @param header The ERF header.
 * @param num_entries The number of entries to write.
 * @param entries The entries to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_write(int fd, const cnwn_ERFHeader * header, int num_entries, const cnwn_ERFEntry * entries);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_fd The output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract(int fd, uint32_t offset, uint32_t size, int output_fd);

/**
 * Read the header and entries from an ERF file.
 * @param path A path to the ERF file.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Return entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_path(const char * path, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Write the header and entries to an ERF file.
 * @param path A path to the ERF file.
 * @param header The ERF header.
 * @param num_entries The number of entries to write.
 * @param entries The entries to write.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_write_path(const char * path, const cnwn_ERFHeader * header, int num_entries, const cnwn_ERFEntry * entries);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_fd The output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract(int fd, uint32_t offset, uint32_t size, int output_fd);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_path A path to the output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract_path(int fd, uint32_t offset, uint32_t size, const char * output_path);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param path A path to the ERF file.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_path A path to the output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract_path2(const char * path, uint32_t offset, uint32_t size, const char * output_path);


#ifdef __cplusplus
}
#endif

#endif
