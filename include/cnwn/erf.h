/**
 * @file erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/file_system.h"
#include "cnwn/endian.h"
#include "cnwn/resource.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ERF resource handler.
 */
extern CNWN_PUBLIC const cnwn_ResourceHandler CNWN_RESOURCE_HANDLER_ERF;

/**
 * Parse a version string as found in an ERF file.
 * @param s The version string.
 * @returns The version, will be zeroed for invalid values.
 */
extern CNWN_PUBLIC cnwn_Version cnwn_erf_parse_version(const char * s);

/**
 * Default handler for ERF files.
 * @param resource The resource struct to initialize.
 * @param f The file, must be set at the correct offset.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_resource_init_from_file_erf(cnwn_Resource * resource, cnwn_File * f);

/**
 * Default handler for ERF files.
 * @param resource Only deinitialize the resource type specific data.
 */
extern CNWN_PUBLIC void cnwn_resource_deinit_erf(cnwn_Resource * resource);

/**
 * Default handler for ERF files.
 * @param resource The resource.
 * @returns The number of meta files.
 */
extern CNWN_PUBLIC int cnwn_resource_get_num_meta_files_erf(const cnwn_Resource * resource);

/**
 * Default handler for ERF files.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param[out] ret_meta_file Return the meta file.
 * @returns The number of returned meta files or zero if @p index is out of range.
 */
extern CNWN_PUBLIC int cnwn_resource_get_meta_file_erf(const cnwn_Resource * resource, int index, cnwn_MetaFile * ret_meta_file);

/**
 * Default handler for ERF files.
 * @param resource The resource.
 * @param index The index of the meta file, negative values will wrap from the end.
 * @param input_f The file to read the resource meta file from.
 * @param output_f The file to write the resource meta file to.
 * @returns The number of bytes written to @p output_f or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_resource_meta_file_extract_erf(const cnwn_Resource * resource, int index, cnwn_File * input_f, cnwn_File * output_f);

#ifdef __cplusplus
}
#endif

#endif
