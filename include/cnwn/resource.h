/**
 * @file resource.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_H
#define CNWN_RESOURCE_H

#include "cnwn/endian.h"
#include "cnwn/containers.h"
#include "cnwn/file_system.h"
#include "cnwn/resource_type.h"

/**
 * Check if a char is valid for a resource name.
 * @param c The char.
 * @returns True or false.
 */
#define CNWN_RESOURCE_NAME_CHAR_VALID(c) (((c) >= 'a' && (c) <= 'z') || ((c) >= 'A' && (c) <= 'Z') || ((c) >= '0' && (c) <= '9') || (c) == '_')

/**
 * Get a handler for a specific resource type.
 * @param t The resource type.
 * @returns A handler or NULL if @p t is invalid.
 */
#define CNWN_RESOURCE_HANDLER(t) (CNWN_RESOURCE_TYPE_VALID(t) ? CNWN_RESOURCE_HANDLERS + (t) : NULL)

/**
 * @see cnwn_Array
 */
typedef cnwn_Array cnwn_MetaFileArray;

/**
 * @see cnwn_Array
 */
typedef cnwn_Array cnwn_ResourceArray;

/**
 * @see struct cnwn_MetaFile_s
 */
typedef struct cnwn_MetaFile_s cnwn_MetaFile;

/**
 * @see struct cnwn_ResourceCallbacks_s
 */
typedef struct cnwn_ResourceCallbacks_s cnwn_ResourceCallbacks;

/**
 * @see struct cnwn_ResourceERF_s
 */
typedef struct cnwn_ResourceERF_s cnwn_ResourceERF;

/**
 * @see struct cnwn_Resource_s
 */
typedef struct cnwn_Resource_s cnwn_Resource;

/**
 * @see struct cnwn_ResourceHandler_s
 */
typedef struct cnwn_ResourceHandler_s cnwn_ResourceHandler;

/**
 * Initialize a resource from file, usually when reading to extract from ERF/BIF.
 * @param resource The resource struct to initialize.
 * @param input_f The input file to read from, must be at the correct offset which will be stored in the resource.
 * @returns Zero on success and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Callback implementations only need to worry about the resource type specific data and modifying the the meta filenames.
 */
typedef int (*cnwn_ResourceInitFromFile)(cnwn_Resource * resource, cnwn_File * input_f);

/**
 * Initialize a resource from path, usually when archiving to ERF/BIF.
 * @param resource The resource struct to initialize.
 * @param path The path to the file to read from.
 * @returns Zero on success and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will not actually read any contents of @p path.
 * @note Callback implementations only need to worry about the resource type specific data and modifying the the meta filenames.
 */
typedef int (*cnwn_ResourceInitFromPath)(cnwn_Resource * resource, const char * path);

/**
 * Deinitialize a resource.
 * @param resource The resource deinitialize.
 * @note Callback implementations should only free the resource type specific data.
 */
typedef void (*cnwn_ResourceDeinit)(cnwn_Resource * resource);

/**
 * Extract a resource (binary).
 * @param resource The resource.
 * @param input_f The file to read the resource from, will seek to the right offset based on the resource->offset.
 * @param output_f The file to write the resource to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceExtract)(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Archive a resource (binary).
 * @param resource The resource.
 * @param input_f The file to read the resource from.
 * @param output_f The file to write the resource to, will seek to the right offset based on the resource->offset.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceArchive)(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Extract a resource (binary) meta file.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param input_f The file to read the resource meta file from.
 * @param output_f The file to write the resource meta file to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceMetaFileExtract)(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Archive a resource (binary) meta file.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param input_f The file to read the resource meta file from.
 * @param output_f The file to write the resource meta file to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int64_t (*cnwn_ResourceMetaFileArchive)(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Callbacks for resource type specific handlers.
 */
struct cnwn_ResourceCallbacks_s {

    /**
     * @see cnwn_ResourceInitFromFile
     */
    cnwn_ResourceInitFromFile f_init_from_file;

    /**
     * @see cnwn_ResourceInitFromPath
     */
    cnwn_ResourceInitFromPath f_init_from_path;

    /**
     * @see cnwn_ResourceDeinit
     */
    cnwn_ResourceDeinit f_deinit;

    /**
     * @see cnwn_ResourceExtract
     */
    cnwn_ResourceExtract f_extract;

    /**
     * @see cnwn_ResourceArchive
     */
    cnwn_ResourceArchive f_archive;

    /**
     * @see cnwn_ResourceMetaFileExtract
     */
    cnwn_ResourceMetaFileExtract f_meta_file_extract;

    /**
     * @see cnwn_ResourceMetaFileArchive
     */
    cnwn_ResourceMetaFileArchive f_meta_file_archive;
};

/**
 * Files that are not pure resources, but can be used as part of resources.
 */
struct cnwn_MetaFile_s {

    /**
     * The name of the meta file.
     */
    char * name;

    /**
     * A description of the meta file.
     */
    char * description;

    /**
     * The size of the meta file (in bytes).
     */
    int64_t size;
};

/**
 * ERF specific data.
 */
struct cnwn_ResourceERF_s {
    
    /**
     * The ERF type (as represented in a file).
     */
    char typestr[5];

    /**
     * The ERF version (as represented in a file).
     */
    char versionstr[5];

    /**
     * Version.
     */
    cnwn_Version version;

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
     * The name.
     */
    char * name;

    /**
     * The path to the resource.
     */
    char * path;
    
    /**
     * Offset in a file.
     */
    int64_t offset;

    /**
     * Resource size (in bytes).
     */
    int64_t size;
    
    /**
     * A parent or NULL if top.
     */
    cnwn_Resource * parent;
    
    /**
     * The child resources.
     */
    cnwn_ResourceArray resources;

    /**
     * The subresources.
     */
    cnwn_MetaFileArray meta_files;

    /**
     * Resource specific data.
     */
    union {

        /**
         * ERF (erf, hak, mod and nwm files).
         */
        cnwn_ResourceERF r_erf;
    } r;
};

/**
 * A resource handler.
 */
struct cnwn_ResourceHandler_s {

    /**
     * The name of the handler.
     */
    const char * name;

    /**
     * The callbacks.
     */
    cnwn_ResourceCallbacks callbacks;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Handlers for different types.
 */
extern CNWN_PUBLIC cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE];

/**
 * Check if a resource name is valid or not.
 * @param name The resource name.
 * @param version A version or NULL to always return true.
 * @returns True or false.
 * @note This function will always return true if @p version is NULL and will return false if an unsupported @p version is specified.
 *
 * A valid name: max 16 (32 for 1.1) characters, only ascii alpha, digits and underscore allowed.
 */
extern CNWN_PUBLIC bool cnwn_resource_name_valid(const char * name, const cnwn_Version * version);

/**
 * Initialize a meta file struct.
 * @param meta_file The meta file struct to initialize.
 * @param name The name of the meta file.
 * @param description The description of the meta file.
 * @param size The size of the meta file.
 */
extern CNWN_PUBLIC void cnwn_meta_file_init(cnwn_MetaFile * meta_file, const char * name, const char * description, int64_t size);

/**
 * Deinitialize a meta file.
 * @param meta_file The meta file to deinitialize.
 */
extern CNWN_PUBLIC void cnwn_meta_file_deinit(cnwn_MetaFile * meta_file);

/**
 * Get the name from a meta file.
 * @param meta_file The meta file.
 * @returns The name.
 */
extern CNWN_PUBLIC const char * cnwn_meta_file_get_name(const cnwn_MetaFile * meta_file);

/**
 * Get the description from a meta file.
 * @param meta_file The meta file.
 * @returns The description.
 */
extern CNWN_PUBLIC const char * cnwn_meta_file_get_description(const cnwn_MetaFile * meta_file);

/**
 * Vanilla initialization of a resource.
 * @param resource The resource struct to initialize.
 * @param type The resource type.
 * @param name The name of the resource.
 * @param offset The offset in a file (zero if the file wasn't initialized from file).
 * @param size The size of the resource.
 * @param parent The parent or NULL if the resource is a top resource.
 * @returns Zero on success and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t offset, int64_t size, cnwn_Resource * parent);

/**
 * Initialize a resource from file, usually when reading to extract from ERF/BIF.
 * @param resource The resource struct to initialize.
 * @param type The resource type.
 * @param name The name of the resource.
 * @param offset The offset in a file (zero if the file wasn't initialized from file).
 * @param size The size of the resource.
 * @param parent The parent or NULL if the resource is a top resource.
 * @param input_f The input file to read from, must be at the correct offset which will be stored in the resource.
 * @returns Zero on success and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_from_file(cnwn_Resource * resource, cnwn_ResourceType type, const char * name, int64_t size, cnwn_Resource * parent, cnwn_File * input_f);

/**
 * Initialize a resource from path, usually when archiving to ERF/BIF.
 * @param resource The resource struct to initialize.
 * @param parent The parent or NULL if the resource is a top resource.
 * @param path The path to the file to read from.
 * @returns Zero on success and a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note This function will not actually read any contents of @p path.
 */
extern CNWN_PUBLIC int cnwn_resource_init_from_path(cnwn_Resource * resource, cnwn_Resource * parent, const char * path);

/**
 * Deinitialize a resource.
 * @param resource The resource deinitialize.
 */
extern CNWN_PUBLIC void cnwn_resource_deinit(cnwn_Resource * resource);

/**
 * Get the resource type.
 * @param resource The resource.
 * @returns The resource type.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_get_type(const cnwn_Resource * resource);

/**
 * Get the resource type info.
 * @param resource The resource.
 * @returns The resource type info (may contain invalid data, check info->type).
 */
extern CNWN_PUBLIC cnwn_ResourceTypeInfo cnwn_resource_get_type_info(const cnwn_Resource * resource);

/**
 * Get the resource name.
 * @param resource The resource.
 * @returns The resource name (may be empty but never NULL).
 */
extern CNWN_PUBLIC const char * cnwn_resource_get_name(const cnwn_Resource * resource);

/**
 * Get the resource path.
 * @param resource The resource.
 * @returns The resource path, will return an empty string if the resource has no path associated with it.
 */
extern CNWN_PUBLIC const char * cnwn_resource_get_path(const cnwn_Resource * resource);

/**
 * Get the number of resources.
 * @param resource The resource.
 * @returns The number of resources this resource has.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_resources(const cnwn_Resource * resource);

/**
 * Get a child resource from the resource.
 * @param resource The resource.
 * @param index The index of the resource, negative values will wrap from the end.
 * @returns The child resource or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC cnwn_Resource * cnwn_resource_get_resource(const cnwn_Resource * resource, int index);

/**
 * Extract a resource (binary).
 * @param resource The resource.
 * @param input_f The file to read the resource from, will seek to the right offset based on the resource->offset.
 * @param output_f The file to write the resource to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Archive a resource (binary).
 * @param resource The resource.
 * @param input_f The file to read the resource from.
 * @param output_f The file to write the resource to, will seek to the right offset based on the resource->offset.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_archive(const cnwn_Resource * resource, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Get the number of meta files the resource has.
 * @param resource The resource.
 * @returns The number of meta files.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_meta_files(const cnwn_Resource * resource);

/**
 * Get a meta file from the resource.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @returns The meta file or NULL if @p index is out of range.
 */
extern CNWN_PUBLIC const cnwn_MetaFile * cnwn_resource_get_meta_file(const cnwn_Resource * resource, int index);

/**
 * Extract a resource (binary) meta file.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param input_f The file to read the resource meta file from.
 * @param output_f The file to write the resource meta file to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_meta_file_extract(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f);

/**
 * Archive a resource (binary) meta file.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param input_f The file to read the resource meta file from.
 * @param output_f The file to write the resource meta file to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_meta_file_archive(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f);

#ifdef __cplusplus
}
#endif

#endif
