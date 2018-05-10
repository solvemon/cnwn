/**
 * @file resource.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_H
#define CNWN_RESOURCE_H

#include "cnwn/endian.h"
#include "cnwn/file_system.h"
#include "cnwn/resource_type.h"
#include "cnwn/containers.h"
//#include "erf.h"

/**
 * Get a resource handler for a specific type.
 */
#define CNWN_RESOURCE_HANDLER(t) ((t) > CNWN_RESOURCE_TYPE_INVALID && (t) < CNWN_MAX_RESOURCE_TYPE ? CNWN_RESOURCE_HANDLERS[(t)] : CNWN_RESOURCE_HANDLER_NONE)

/**
 * @see struct cnwn_ResourceERF_s
 */
typedef struct cnwn_ResourceERF_s cnwn_ResourceERF;

/**
 * @see struct cnwn_Resource_s
 */
typedef struct cnwn_Resource_s cnwn_Resource;

/**
 * @see struct cnwn_Array_s
 */
typedef cnwn_Array cnwn_ResourceArray;

/**
 * @see struct cnwn_ResourceItem_s
 */
typedef struct cnwn_ResourceItem_s cnwn_ResourceItem;

/**
 * @see struct cnwn_ResourceHandler_s
 */
typedef struct cnwn_ResourceHandler_s cnwn_ResourceHandler;

/**
 * Initialize a resource from a file.
 * @param resource The resource struct to initialize.
 * @param size The size of the resource in bytes.
 * @param f The file, must be set at the correct offset.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int (*cnwn_ResourceInitExtract)(cnwn_Resource * resource, int64_t size, cnwn_File * f);

/**
 * Initialize a resource from a path, usually for archiving stuff.
 * @param resource The resource struct to initialize.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int (*cnwn_ResourceInitArchive)(cnwn_Resource * resource);

/**
 * Used to deinitialize resources.
 * @param resource Only deinitialize the resource type specific data.
 */
typedef void (*cnwn_ResourceDeinit)(cnwn_Resource * resource);

/**
 * Get the number of items for input/output.
 * @param resource The resource to get number of items from.
 * @returns The number of items.
 */
typedef int (*cnwn_ResourceGetNumItems)(const cnwn_Resource * resource);

/**
 * Get an item (a filename that will be used to input/output specific data for the resource).
 * @param resource The resource to get item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param[out] ret_item Return the item here.
 * @returns The number of returned items (will be zero if the resource has no items or @p index is out of range).
 */
typedef int (*cnwn_ResourceGetItem)(const cnwn_Resource * resource, int index, cnwn_ResourceItem * ret_item);

/**
 * Extract an item from the resource.
 * @param resource The resource to extract an item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param source_f The source data of the resource.
 * @param destination_f Write the extracted item to this file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceExtractItem)(const cnwn_Resource * resource, int index, cnwn_File * source_f, cnwn_File * destination_f);

/**
 * Extract the resource itself.
 * @param resource The resource to extract.
 * @param source_f The source data of the resource.
 * @param destination_f Write the extracted item to this file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceExtract)(const cnwn_Resource * resource, cnwn_File * source_f, cnwn_File * destination_f);

/***
 * Represents an ERF (also mod, nwm and hak) file.
 */
struct cnwn_ResourceERF_s {
    
    /**
     * The ERF type.
     */
    char erf_type[5];

    /**
     * The type as a number.
     */
    uint16_t type;

    /**
     * The ERF version.
     */
    char erf_version[5];

    /**
     * Major version as a number.
     */
    int major_version;

    /**
     * Minor version as a number.
     */
    int minor_version;
    
    /**
     * The number of localized strings.
     */
    uint32_t num_localized_strings;

    /**
     * The localized strings offset.
     */
    uint32_t localized_strings_offset;

    /**
     * The localized strings size.
     */
    uint32_t localized_strings_size;

    /**
     * Keys offset.
     */
    uint32_t keys_offset;

    /**
     * Values offset.
     */
    uint32_t values_offset;

    /**
     * Year.
     */
    uint32_t year;

    /**
     * Day of year.
     */
    uint32_t day_of_year;

    /**
     * Descripting stringref.
     */
    uint32_t description_strref;
    
    /**
     * Resources offset.
     */
    uint32_t resources_offset;

    /**
     * The rest of the header.
     */
    uint8_t rest[116];
};

/**
 * A resource.
 */
struct cnwn_Resource_s {

    /**
     * Type.
     */
    cnwn_ResourceType type;

    /**
     * A parent or NULL if top.
     */
    cnwn_Resource * parent;

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
     * The subresources.
     */
    cnwn_ResourceArray resources;

    /**
     * Resource type specific data.
     */
    union {

        /**
         * ERF resource.
         */
        cnwn_ResourceERF r_erf;
    } r;
};

/**
 * A resource item.
 */
struct cnwn_ResourceItem_s {

    /**
     * The filename of the item.
     */
    char filename[CNWN_PATH_MAX_SIZE];

    /**
     * The size of the item.
     */
    int64_t size;
};

/**
 * A resource handler
 */
struct cnwn_ResourceHandler_s {

    /**
     * When initializing from files.
     */
    cnwn_ResourceInitExtract f_init_extract;

    /**
     *
     */
    cnwn_ResourceInitArchive f_init_archive;

    /**
     * When deinitializing.
     */
    cnwn_ResourceDeinit f_deinit;

    /**
     * When getting the number of items.
     */
    cnwn_ResourceGetNumItems f_get_num_items;

    /**
     * When getting an item.
     */
    cnwn_ResourceGetItem f_get_item;

    /**
     * When extracting items from a resource.
     */
    cnwn_ResourceExtractItem f_extract_item;

    /**
     * Extract the resource.
     */
    cnwn_ResourceExtract f_extract;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Just a zeroed resource handler.
 */
extern CNWN_PUBLIC const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_NONE;

/**
 * The resource handlers.
 */
extern CNWN_PUBLIC cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE];

/**
 * Initialize a resource from a file, usually for listing or extracting stuff.
 * @param resource The resource struct to initialize.
 * @param type The type.
 * @param name The name.
 * @param size The size of the resource in bytes.
 * @param f The file, must be set at the correct offset.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_extract(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f);

/**
 * Initialize a resource from a file, usually for listing or extracting stuff.
 * @param resource The resource struct to initialize.
 * @param type The type.
 * @param name The name.
 * @param path The path to the file or directory to archive.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_archive(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, const char * path);

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
 * Get the resource filename (name + dot + extension).
 * @param resource The resource to get the filename from.
 * @param max_size The maximum size (including zero terminator) of the return string.
 * @param[out] ret_filename Write filename here, pass NULL to get required string length.
 * @returns The string length (excluding zero terminator) of the return string.
 */
extern CNWN_PUBLIC int cnwn_resource_get_filename(const cnwn_Resource * resource, int max_size, char * ret_filename);

/**
 * Get the number of items for input/output.
 * @param resource The resource to get number of items from.
 * @returns The number of items.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_items(const cnwn_Resource * resource);

/**
 * Get an item (a filename that will be used to input/output specific data for the resource).
 * @param resource The resource to get item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param[out] ret_item Return the item here.
 * @returns The number of returned items (will be zero if the resource has no items or @p index is out of range).
 */
extern CNWN_PUBLIC int cnwn_resource_get_item(const cnwn_Resource * resource, int index, cnwn_ResourceItem * ret_item);

/**
 * Extract an item from the resource.
 * @param resource The resource to extract an item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param source_f The source data of the resource.
 * @param destination_f Write the extracted item to this file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract_item(const cnwn_Resource * resource, int index, cnwn_File * source_f, cnwn_File * destination_f);

/**
 * Extract the resource itself.
 * @param resource The resource to extract.
 * @param source_f The source data of the resource.
 * @param destination_f Write the extracted item to this file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * source_f, cnwn_File * destination_f);


/**
 * Get the number of resources for input/output.
 * @param resource The resource to get number of resources from.
 * @returns The number of resources.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_resources(const cnwn_Resource * resource);

/**
 * Get an resource (a filename that will be used to input/output specific data for the resource).
 * @param resource The resource to get resource from.
 * @param index The index of the resource, negative values will wrap from the end.
 * @returns The subresource or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC cnwn_Resource * cnwn_resource_get_resource(const cnwn_Resource * resource, int index);

#ifdef __cplusplus
}
#endif

#endif
