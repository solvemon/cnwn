/**
 * @file erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/common.h"
#include "cnwn/file_system.h"
#include "cnwn/resource_type.h"

/**
 * The maximum number of string entries an ERF can contain.
 */
#define CNWN_ERF_STRING_ENTRIES_MAX 65535

/**
 * The maximum size of a string in a string entry.
 */
#define CNWN_ERF_STRING_ENTRY_MAX_SIZE 65535

/**
 * The maximum number of resource entries an ERF can contain.
 */
#define CNWN_ERF_RESOURCE_ENTRIES_MAX 65535

/**
 * Check if the ERF type is valid (> CNWN_ERF_TYPE_NONE && < CNWN_MAX_ERF_TYPE).
 * @param t The ERF type.
 * @returns True or false.
 */
#define CNWN_ERF_TYPE_VALID(t) ((t) > CNWN_ERF_TYPE_NONE && (t) < CNWN_MAX_ERF_TYPE)

/**
 * Get a string representation of an erf type.
 * @param t The ERF type.
 * @returns A pointer to a string representation or empty string if @p t is invalid.
 */
#define CNWN_ERF_TYPE_STRING(t) ((t) >= CNWN_ERF_TYPE_NONE && (t) < CNWN_MAX_ERF_TYPE ? CNWN_ERF_TYPE_STRINGS[(t)] : "")

/**
 * What type of ERF file.
 */
enum cnwn_ErfType_e {

    /**
     * Invalid/unknown ERF.
     */
    CNWN_ERF_TYPE_NONE = 0,
     
    /**
     * Module.
     */
    CNWN_ERF_TYPE_MODULE,
    
    /**
     * Max enum.
     */
    CNWN_MAX_ERF_TYPE,
};

/**
 * @see enum cnwn_ErfType_e
 */
typedef enum cnwn_ErfType_e cnwn_ErfType;

/**
 * @see struct cnwn_ErfVersion_s
 */
typedef struct cnwn_ErfVersion_s cnwn_ErfVersion;

/**
 * @see struct cnwn_ErfHeader_s
 */
typedef struct cnwn_ErfHeader_s cnwn_ErfHeader;

/**
 * @see struct cnwn_ErfStringEntry_s
 */
typedef struct cnwn_ErfStringEntry_s cnwn_ErfStringEntry;

/**
 * @see struct cnwn_ErfResourceEntry_s
 */
typedef struct cnwn_ErfResourceEntry_s cnwn_ErfResourceEntry;

/**
 * Version of the ERF file.
 */
struct cnwn_ErfVersion_s {

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
 * The info.
 */
struct cnwn_ErfHeader_s {

    /**
     * The type.
     */
    cnwn_ErfType type;

    /**
     * The version.
     */
    cnwn_ErfVersion version;

    /**
     * The number of localized string entries.
     */
    int num_string_entries;

    /**
     * The string entries.
     */
    cnwn_ErfStringEntry * string_entries;

    /**
     * The number of resources entries.
     */
    int num_resource_entries;

    /**
     * The resource entries.
     */
    cnwn_ErfResourceEntry * resource_entries;
    
    /**
     * Some internal data.
     */
    struct {
        /**
         * The type as it was read from ERF.
         */
        char type_str[5];
    
        /**
         * The version as it was read from ERF.
         */
        char version_str[5];
    
        /**
         * The size of the localized strings.
         */
        int64_t string_entries_size;

        /**
         * The offset of the localized strings.
         */
        int64_t string_entries_offset;

        /**
         * The keys offset.
         */
        int64_t keys_offset;
    
        /**
         * The values offset.
         */
        int64_t values_offset;
    } internal;
};

/**
 * A localized string.
 */
struct cnwn_ErfStringEntry_s {

    /**
     * Language ID.
     */
    uint32_t lang_id;
    
    /**
     * The offset of the string.
     */
    int64_t offset;

    /**
     * The size of the string.
     */
    int64_t size;

    /**
     * Internal data.
     */
    struct {

        /**
         * The offset of the entry.
         */
        int64_t entry_offset;
        
    } internal;
};
    
/**
 * An resource.
 */
struct cnwn_ErfResourceEntry_s {

    /**
     * The key of the resource.
     */
    cnwn_ResourceType type;
    
    /**
     * The key of the entry.
     */
    char * key;

    /**
     * A combination of key + resource type as filename extension (except for CNWN_RESOURCE_TYPE_NONE which yields no extension).
     */
    char * filename;
    
    /**
     * The offset of the resource.
     */
    int64_t offset;

    /**
     * The size of the resource.
     */
    int64_t size;

    /**
     * Some internal data.
     */
    struct {

        /**
         * The internal type as used by Aurora.
         */
        uint32_t id;
        
        /**
         * The internal type as used by Aurora.
         */
        uint16_t type;

        /**
         * Unused data.
         */
        uint16_t unused;
        
        /**
         * The offset to the key.
         */
        int64_t key_offset;
        
        /**
         * The offset to the value.
         */
        int64_t value_offset;
    } internal;
    
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ERF type strings.
 */
extern CNWN_PUBLIC const char * CNWN_ERF_TYPE_STRINGS[CNWN_MAX_ERF_TYPE];

/**
 * Get an ERF type based of a string such as you would find it in an ERF.
 * @param s The string to find an ERF type for.
 * @returns The ERF type or CNWN_ERF_TYPE_NONE if @p s does not contain a valid ERF type.
 **/
extern CNWN_PUBLIC cnwn_ErfType cnwn_erf_type_parse(const char * s);

/**
 * Get an ERF version based of a string such as you would find it in an ERF.
 * @param s The string to find an ERF version for.
 * @returns The ERF version, if @p s is invalid both major and minor will be set to zero.
 **/
extern CNWN_PUBLIC cnwn_ErfVersion cnwn_erf_version_parse(const char * s);

/**
 * Check if the version is supported.
 * @param version The version to check.
 * @returns True or false.
 **/
extern CNWN_PUBLIC bool cnwn_erf_version_support(const cnwn_ErfVersion * version);

/**
 * Read the top of an ERF header.
 * @param f The file to read from.
 * @param[out] ret_header Write results to this header.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_erf_header_read_top(cnwn_File * f, cnwn_ErfHeader * ret_header);

/**
 * Read a string entry from a file.
 * @param f The file to read from (must be at the offset of the string entry).
 * @param[out] ret_entry Write results to this string entry.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_erf_header_read_string_entry(cnwn_File * f, cnwn_ErfStringEntry * ret_entry);

/**
 * Read a resource entry key from a file.
 * @param f The file to read from (must be at the offset of the resource entry key).
 * @param version Used to determine the key size.
 * @param[out] ret_entry Write results to this resource entry.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_erf_header_read_resource_entry_key(cnwn_File * f, const cnwn_ErfVersion * version, cnwn_ErfResourceEntry * ret_entry);

/**
 * Read a resource entry value from a file.
 * @param f The file to read from (must be at the offset of the resource entry value).
 * @param[out] ret_entry Write results to this resource entry.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_erf_header_read_resource_entry_value(cnwn_File * f, cnwn_ErfResourceEntry * ret_entry);

/**
 * Free an ERF header.
 * @param header The header to free.
 **/
extern CNWN_PUBLIC void cnwn_erf_header_free(cnwn_ErfHeader * header);

/**
 * Read header from an ERF.
 * @param f The file to read from, expects an offset at the start of the ERF.
 * @param[out] ret_header Return a pointer to a newly allocated header here, must be freed using cnwn_erf_header_free().
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note The returned number of read bytes is no indication of what the offset of @p f is after the function has been called.
 **/
extern CNWN_PUBLIC int64_t cnwn_erf_header_read(cnwn_File * f, cnwn_ErfHeader ** ret_header);

/**
 * Summarize the total size of all resource entries.
 * @param header The header to use for the summary.
 * @returns The length number of bytes for all resources.
 **/
extern CNWN_PUBLIC int64_t cnwn_erf_header_sum_resources(const cnwn_ErfHeader * header);

#ifdef __cplusplus
}
#endif

#endif
