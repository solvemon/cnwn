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

/**
 * @see struct cnwn_Resource_s
 */
typedef struct cnwn_Resource_s cnwn_Resource;

/**
 * @see struct cnwn_ResourceIterator_s
 */
typedef struct cnwn_ResourceIterator_s cnwn_ResourceIterator;

/**
 * @see struct cnwn_ResourceIteratorItem_s
 */
typedef struct cnwn_ResourceIteratorItem_s cnwn_ResourceIteratorItem;

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
     * The filenames.
     */
    cnwn_StringArray filenames;

    /**
     * The entry filename.
     */
    char * entry_filename;

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

/**
 * Iterate resources.
 */
struct cnwn_ResourceIterator_s {

    /**
     * The number of resource iterator items.
     */
    int num_items;
    
    /**
     * The resource iterator items.
     */
    cnwn_ResourceIteratorItem * items;
};

/**
 * A resource iterator item.
 */
struct cnwn_ResourceIteratorItem_s {

    /**
     * The resource.
     */
    const cnwn_Resource * resource;

    /**
     * The index of the resource's filename for this item.
     */
    int filename_index;

    /**
     * The path to the file as it related to subresources etc.
     */
    char * path;
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
extern CNWN_PUBLIC int cnwn_resource_init_read_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_File * f);

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
 * Get the number of filenames for input/output.
 * @param resource The resource to get number of filenames from.
 * @returns The number of filenames.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_filenames(const cnwn_Resource * resource);

/**
 * Get the filename for input/output.
 * @param resource The resource to get filename from.
 * @param index The index of the filename, negative values will wrap from the end.
 * @returns The filename or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC const char * cnwn_resource_get_filename(const cnwn_Resource * resource, int index);

/**
 * Get the entry filename for input/output.
 * @param resource The resource to get entry filename from.
 * @returns The entry filename or NULL if @p index is out of range.
 *
 * The entry filename merely describes the resource name + extension.
 */
extern CNWN_PUBLIC const char * cnwn_resource_get_entry_filename(const cnwn_Resource * resource);

/**
 * Extract a resource to a new file.
 * @param resource The resource to extract from.
 * @param input_f The file to read from, the offset will be seeked to resource->offset.
 * @param path Where to extract the resource, if directories are involved they will be created.
 * @returns The number of written bytes or a negative value on error.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * input_f, const char * path);

/**
 * Copy a resource from one file to another.
 * @param resource The resource to copy.
 * @param input_f The file to read from, make sure it's offset is where it's supposed to before calling this function.
 * @param path A path to the output file, will be created (and truncate any existing) file. If any directories are in the path they will be created.
 * @returns The number of written bytes or a negative value on error.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_copy2(const cnwn_Resource * resource, cnwn_File * input_f, const char * path);

/**
 * Initialize a new resource iterator.
 * @param iterator The resource iterator struct to initialize.
 * @param resource The resource to iterate (will iterate all of it's subresources too, if any).
 * @returns The number of items in the iterator.
 */
extern CNWN_PUBLIC int cnwn_resource_iterator_init(cnwn_ResourceIterator * iterator, const cnwn_Resource * resource);

/**
 * Deinitialize a resource iterator.
 * @param iterator The resource iterator to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_resource_iterator_deinit(cnwn_ResourceIterator * iterator);

/**
 * Get the length (number of items) in the iterator.
 * @param iterator The resource iterator to get the length for.
 * @returns The number of items in the iterator.
 */
extern CNWN_PUBLIC int cnwn_resource_iterator_get_length(const cnwn_ResourceIterator * iterator);

/**
 * Get a resource item from the iterator.
 * @param iterator The resource iterator to get the item from.
 * @param index The index of the item to get, negative values will wrap from the end of the iterator.
 * @returns A resource item or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC const cnwn_ResourceIteratorItem * cnwn_resource_iterator_get(const cnwn_ResourceIterator * iterator, int index);


#ifdef __cplusplus
}
#endif

#endif
