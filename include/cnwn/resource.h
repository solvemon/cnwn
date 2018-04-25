/**
 * @file resource.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_H
#define CNWN_RESOURCE_H

#include "cnwn/resource_type.h"

/**
 * @see struct cnwn_ResourceHandler_s
 */
typedef struct cnwn_ResourceHandler_s cnwn_ResourceHandler;

/**
 * Used to extract a specific resource.
 * @param resource_type Let the handler know what type the caller is providing.
 * @param f The input file, must be at the offset of the resource data to be read.
 * @param size The size (in the ERF file) of the resource to extract.
 * @param output_f Write output to this file.
 * @returns The number of written bytes or a negative value on error.
 */
typedef int64_t (*cnwn_ResourceExtract)(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * output_f);

/**
 * Used to archive a specific resource.
 * @param resource_type Let the handler know what type the caller is providing.
 * @param f The input file, must be at the offset of where the resource data is to be written.
 * @param size The size of the resource to archive in the input file.
 * @param input_f Read input from this file.
 * @returns The number of written bytes or a negative value on error.
 */
typedef int64_t (*cnwn_ResourceArchive)(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * input_f);

/**
 * Handling extraction/archiving of resources.
 */
struct cnwn_ResourceHandler_s {

    /**
     * Neet human readable description.
     **/
    const char * description;

    /**
     * Extract implementation.
     */
    cnwn_ResourceExtract f_extract;

    /**
     * Archive implementation.
     */
    cnwn_ResourceArchive f_archive;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The currently installed handlers.
 * Leaving it public just in case someone wants fiddle or hook into it during runtime.
 */
extern CNWN_PUBLIC cnwn_ResourceHandler CNWN_RESOURCE_HANDLERS[CNWN_MAX_RESOURCE_TYPE];

/**
 * Uninstall all handlers.
 */
extern CNWN_PUBLIC void cnwn_resource_uninstall_handlers(void);

/**
 * Install the XML handlers.
 * @note This may overwrite old handlers, but will not touch the others.
 */
extern CNWN_PUBLIC void cnwn_resource_install_xml_handlers(void);

/**
 * Extract a resource.
 * @param resource_type The type of the resource we intend to extract.
 * @param f The file to extract from.
 * @param size The size of the resource.
 * @param output_f The resource output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Will use the appropriate handler or a binary copy if no handler is found.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_extract(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * output_f);

/**
 * Archive a resource.
 * @param resource_type The type of the resource we intend to extract.
 * @param f The file to archive to.
 * @param size The size of the resource.
 * @param input_f The file to read the resource from.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Will use the appropriate handler or a binary copy if no handler is found.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_archive(cnwn_ResourceType resource_type, cnwn_File * f, int64_t size, cnwn_File * input_f);


#ifdef __cplusplus
}
#endif

#endif
