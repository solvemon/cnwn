/**
 * @file resource.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_H
#define CNWN_RESOURCE_H

#include "cnwn/restypes/erf.h"

/**
 * Used to extract a specific resource.
 * @param f The input file, must be at the offset of the resource data to be read.
 * @param size The size (in the ERF file) of the resource to extract.
 * @param output_f Write output to this file.
 * @returns The number of written bytes or a negative value on error.
 */
typedef int64_t (*cnwn_ResourceExtract)(cnwn_File * f, int64_t size, cnwn_File * output_f);

/**
 * Used to archive a specific resource.
 * @param f The input file, must be at the offset of where the resource data is to be written.
 * @param size The size of the resource to archive in the input file.
 * @param input_f Read input from this file.
 * @returns The number of written bytes or a negative value on error.
 */
typedef int64_t (*cnwn_ResourceArchive)(cnwn_File * f, int64_t size, cnwn_File * output_f);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
