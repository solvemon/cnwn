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
 * The maximum size of a key (including zero terminator).
 */
#define CNWN_ERF_KEY_MAX_SIZE 33

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
 * @see struct cnwn_ErfInfo_s
 */
typedef struct cnwn_ErfInfo_s cnwn_ErfInfo;

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
struct cnwn_ErfInfo_s {

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
     * The number of resources entries.
     */
    int num_resource_entries;

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
    int lang_id;
    
    /**
     * The offset in the ERF.
     */
    int64_t offset;

    /**
     * The size of the string.
     */
    int64_t size;
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
     * The key of the resource.
     */
    char key[CNWN_ERF_KEY_MAX_SIZE];

    /**
     * The offset in the ERF.
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
        int internal_type;

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
 * Read header from an ERF.
 * @param f The file to read from, expects an offset at the start of the ERF.
 * @param[out] ret_info Return the info, NULL to omit.
 * @param max_string_entries The maximum number of string entries to read.
 * @param[out] ret_string_entries Return the string entries here, NULL to omit.
 * @param max_resource_entries The maximum number of resource entries to read.
 * @param[out] ret_resource_entries Return the resource entries here, NULL to omit.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 **/
extern CNWN_PUBLIC int64_t cnwn_erf_read_header(cnwn_File * f, cnwn_ErfInfo * ret_info, int max_string_entries, cnwn_ErfStringEntry * ret_string_entries, int max_resource_entries, cnwn_ErfResourceEntry * ret_resource_entries);

/**
 * Read info from an ERF.
 * @param f The file to read from, expects an offset at the start of the ERF.
 * @param info The info.
 * @param num_string_entries The number of string entries.
 * @param string_entries The string entries.
 * @param num_resource_entries The number of resource entries.
 * @param resource_entries The resource entries here.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 **/
extern CNWN_PUBLIC int64_t cnwn_erf_write_header(cnwn_File * f, const cnwn_ErfInfo * info, int num_string_entries, const cnwn_ErfStringEntry * string_entries, int num_resource_entries, const cnwn_ErfResourceEntry * resource_entries);

#ifdef __cplusplus
}
#endif

#endif
