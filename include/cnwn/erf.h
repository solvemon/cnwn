/**
 * @file erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/file_system.h"
#include "cnwn/endian.h"
#include "cnwn/resource.h"

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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ERF resource handler.
 */
extern CNWN_PUBLIC const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_ERF;

/**
 * Default handler for ERF files.
 * @param resource The resource struct to initialize.
 * @param size The size of the resource in bytes.
 * @param f The file, must be set at the correct offset.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_extract_erf(cnwn_Resource * resource, int64_t size, cnwn_File * f);

/**
 * Default handler for ERF files.
 * @param resource The resource struct to initialize.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_archive_erf(cnwn_Resource * resource);

/**
 * Default handler for ERF files.
 * @param resource Only deinitialize the resource type specific data.
 */
extern CNWN_PUBLIC void cnwn_resource_deinit_erf(cnwn_Resource * resource);

/**
 * Default handler for ERF files.
 * @param resource The resource to get number of items from.
 * @returns The number of items.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_items_erf(const cnwn_Resource * resource);

/**
 * Default handler for ERF files.
 * @param resource The resource to get item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param[out] ret_item Return the item here.
 * @returns The number of returned items (will be zero if the resource has no items or @p index is out of range).
 */
extern CNWN_PUBLIC int cnwn_resource_get_item_erf(const cnwn_Resource * resource, int index, cnwn_ResourceItem * ret_item);

/**
 * Default handler for ERF files.
 * @param resource The resource to extract an item from.
 * @param index The index of the item, negative values will wrap from the end.
 * @param source_f The source data of the resource, will be seeked.
 * @param destination_f Write the extracted item to this file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract_item_erf(const cnwn_Resource * resource, int index, cnwn_File * source_f, cnwn_File * destination_f);

#ifdef __cplusplus
}
#endif

#endif
