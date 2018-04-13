/**
 * @file cnwn.h
 * Plz more info!
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
 * Describes an ERF entry type.
 */
enum cnwn_ResourceType_e {

    /**
     * Invalid ERF entry type.
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
 * Contains the header information for an ERF file.
 */
struct cnwn_ERFHeader_s {

    /**
     * The deserialized string type (for debug purposes).
     */
    char type_str[5];
    
    /**
     * The deserialized string version (for debug purposes).
     */
    char version_str[5];
    
    /**
     * What type of ERF file is this.
     */
    cnwn_ResourceType type;

    /**
     * The version of the ERF file.
     */
    cnwn_Version version;

    /**
     * Localized strings.
     */
    struct {

        /**
         * Count.
         */
        uint32_t count;
    
        /**
         * Size.
         */
        uint32_t size;
        
        /**
         * Offset.
         */
        uint32_t offset;
    } localized_strings;

    /**
     * Entries.
     */
    struct {

        /**
         * Count.
         */
        uint32_t count;
        
        /**
         * Offset for the string keys.
         */
        uint32_t keys_offset;
        
        /**
         * Offset for the resources.
         */
        uint32_t resources_offset;
    } entries;

    /**
     * The rest of the header, don't know what it is.
     */
    char rest[128];
};

/**
 * Represents an entry of an ERF file.
 */
struct cnwn_ERFEntry_s {

    /**
     * The key (name) of the entry.
     */
    char key[64];

    /**
     * The offset where the key is found.
     */
    uint32_t key_offset;

    /**
     * Resource ID.
     */
    uint32_t resource_id;

    /**
     * Resource type.
     */
    cnwn_ResourceType resource_type;

    /**
     * The ERF resource type.
     */
    uint16_t erf_type;
    
    /**
     * The offset where the resource is found.
     */
    uint32_t resource_offset;

    /**
     * The size of the resource.
     */
    uint32_t resource_size;
    
    /**
     * Unused.
     */
    uint16_t unused;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Info about each resource type.
 */
extern CNWN_PUBLIC const cnwn_ResourceInfo CNWN_RESOURCE_INFOS[CNWN_MAX_RESOURCE_TYPE];

/**
 * Get a resource type from an ERF resource type (as found in the entries).
 * @param erf_type The entry's resource type.
 * @returns A resource type or CNWN_RESOURCE_TYPE_NONE if @p erf_type is invalid.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_erf_type(int erf_type);

/**
 * Get a resource type from an extension (a 3 letter combination).
 * @param extension The extension to get the resource type from.
 * @returns A resource type or CNWN_RESOURCE_TYPE_NONE if @p extension is invalid.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_extension(const char * extension);

/**
 * Get the last error message.
 */
extern CNWN_PUBLIC const char * cnwn_get_error(void);

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
 * Read an unsigned 32-bit integer from a file.
 * @param fd The file to read from.
 * @param[out] ret_i Return the integer here, pass NULL to omit.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_read_uint32(int fd, uint32_t * ret_i);

/**
 * Write an unsigned 32-bit integer to a file.
 * @param fd The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_write_uint32(int fd, uint32_t i);

/**
 * Read an unsigned 16-bit integer from a file.
 * @param fd The file to read from.
 * @param[out] ret_i Return the integer here, pass NULL to omit.
 * @return The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_read_uint16(int fd, uint16_t * ret_i);

/**
 * Write an unsigned 16-bit integer to a file.
 * @param fd The file to write to.
 * @param i Write this integer.
 * @return The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_write_uint16(int fd, uint16_t i);

#ifdef __cplusplus
}
#endif

#endif
