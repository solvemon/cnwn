/**
 * @file resource.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_H
#define CNWN_RESOURCE_H

#include "cnwn/endian.h"
#include "cnwn/file_system.h"
#include "cnwn/resource_type.h"

/**
 * @see struct cnwn_Resource_s
 */
typedef struct cnwn_Resource_s cnwn_Resource;

/**
 * A resource.
 */
struct cnwn_Resource_s {

    /**
     * Type.
     */
    cnwn_ResourceType type;

    /**
     * The name.
     */
    char * name;

    /**
     * Offset.
     */
    int64_t offset;

    /**
     * Size.
     */
    int64_t size;

    /**
     * Resource type specific data.
     */
    union {

        /**
         * ERF (also MOD and HAK).
         */
        struct {

            /**
             * Major version for ERF format.
             */
            int major_version;

            /**
             * Minor version for ERF format.
             */
            int minor_version;

            /**
             * Year.
             */
            int year;

            /**
             * Day of year.
             */
            int day_of_year;

            /**
             * The number of localized strings.
             */
            int num_localized_strings;            

            /**
             * Localized strings offset.
             */
            int64_t localized_strings_offset;

            /**
             * Localized strings size.
             */
            int64_t localized_strings_size;

            /**
             * Keys offset.
             */
            int64_t keys_offset;

            /**
             * Values offset.
             */
            int64_t values_offset;
            
            /**
             * The number of subresources in the file.
             */
            int num_subresources;

            /**
             * Subresources.
             */
            cnwn_Resource * subresources;
            
        } r_erf;
        
    } r;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Initialize a resource from a path.
 * @param resource The resource struct to initialize.
 * @param type The type.
 * @param name The name.
 * @param size The size of the resource in bytes.
 * @param f The file, must be set at the correct offset.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_from_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f);

/**
 * Deinitialize a resource.
 * @param resource The resource to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_resource_deinit(cnwn_Resource * resource);

/**
 * Get the resource type.
 * @param resource The resource to get the type from.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_get_type(const cnwn_Resource * resource);

/**
 * Get the resource name.
 * @param resource The resource to get the name from.
 */
extern CNWN_PUBLIC const char * cnwn_resource_get_name(const cnwn_Resource * resource);

/**
 * Get the number of subresources this resource has.
 * @param resource The resource to get subresources from.
 * @returns The number of subresources.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_subresources(const cnwn_Resource * resource);

/**
 * Get a subresource from the resource.
 * @param resource The resource to get subresources from.
 * @param index The index of the resource, a negative value will wrap from the end.
 * @returns The subresource or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC const cnwn_Resource * cnwn_resource_get_subresource(const cnwn_Resource * resource, int index);

/**
 * Get all the filenames this resource will read or write to/from.
 * @param resource The resource to get files from.
 * @returns The filenames (NULL terminated string array).
 */
extern CNWN_PUBLIC char ** cnwn_resource_get_filenames(const cnwn_Resource * resource);


#ifdef __cplusplus
}
#endif

#endif
