/**
 * @file erf.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/resource.h"

/**
 * Let's the handler know what mode to operate in.
 */
enum cnwn_ERFHandlerMode_e {
    
    /**
     * Ask the handler to do nothing.
     */
    CNWN_ERF_HANDLER_MODE_NONE = 0,
    
    /**
     * Ask the handler to extract an entry.
     */
    CNWN_ERF_HANDLER_MODE_EXTRACT,

    /**
     * Ask the handler to archive an entry.
     */
    CNWN_ERF_HANDLER_MODE_ARCHIVE,

    /**
     * Ask the handler what files it intends to extract/archive.
     */
    CNWN_ERF_HANDLER_MODE_QUERY,
    
    /**
     * Max enum.
     */
    CNWN_MAX_ERF_HANDLER_MODE
};

/**
 * @see enum cnwn_ERFHandlerMode_e
 */
typedef enum cnwn_ERFHandlerMode_e cnwn_ERFHandlerMode;

/**
 * @see struct cnwn_ERFHeader_s
 */
typedef struct cnwn_ERFHeader_s cnwn_ERFHeader;

/**
 * @see struct cnwn_ERFEntry_s
 */
typedef struct cnwn_ERFEntry_s cnwn_ERFEntry;

/**
 * @see struct cnwn_ERFHandlers_s
 */
typedef struct cnwn_ERFHandlers_s cnwn_ERFHandlers;

/**
 * A handler for writing entries from/to the ERF file.
 * @param handlers The struct from which this handler was set.
 * @param mode The requested mode of operation for the handler, please respect CNWN_ERF_HANDLER_MODE_NONE.
 * @param version The version of the ERF file.
 * @param fd The file descriptor for the ERF file, will be seek'ed to the right position before the handler is called: ONLY READ/WRITE OPERATIONS PERMITTED (no seeking etc).
 * @param entry_path The path of the output/input file.
 * @param resource_type The expected resource type of the entry to extract/archive.
 * @param resource_size The size of the entry resource, should be ignored when CNWN_ERF_HANDLER_MODE_ARCHIVE.
 * @param[out] ret_bytes Return the number of extracted/archived bytes, NULL to omit.
 * @param[out] ret_files Return the paths of the extracted/archived files (must be manually freed using cnwn_free_strings()), NULL to omit.
 * @returns A positive value if handled, zero if ignored or a negative value on error (in which case the implementation should use cnwn_set_error()).
 * @note If a handler returns a negative value further processing of handlers is discontinued by the calling function.
 */
typedef int (*cnwn_ERFHandler)(const cnwn_ERFHandlers * handlers, cnwn_ERFHandlerMode mode, const cnwn_Version * version, int fd, const char * entry_path, cnwn_ResourceType resource_type, int64_t resource_size, int64_t * ret_bytes, char ** ret_files);

/**
 * ERF header.
 */
struct cnwn_ERFHeader_s {

    /**
     * The type of resource the ERF is.
     */
    cnwn_ResourceType type;

    /**
     * The type string as it was read from a file.
     */
    char type_str[5];
    
    /**
     * The version.
     */
    cnwn_Version version;

    /**
     * The version string as it was read from a file.
     */
    char version_str[5];
    
    /**
     * The number of localized strings.
     */
    uint32_t num_localized_strings;

    /**
     * The offset of the localized strings.
     */
    uint32_t localized_strings_offset;
    
    /**
     * The size of the localized strings.
     */
    uint32_t localized_strings_size;
    
    /**
     * The number of entries.
     */
    uint32_t num_entries;

    /**
     * The offset of the entry keys.
     */
    uint32_t keys_offset;
    
    /**
     * The offset for the entry resources.
     */
    uint32_t resources_offset;
};

/**
 * ERF entry.
 */
struct cnwn_ERFEntry_s {

    /**
     * The resource type the entry is.
     */
    cnwn_ResourceType type;

    /**
     * The key (name) of the resource, max 32 characters for V1.1 and 16 for V1.0.
     */
    char key[33];

    /**
     * The offset to the key.
     */
    uint32_t key_offset;
    
    /**
     * The offset to the resource.
     */
    uint32_t resource_offset;

    /**
     * The ID of the resource.
     */
    uint32_t resource_id;

    /**
     * The ERF type of the resource.
     */
    uint16_t erf_type;
    
    /**
     * Some unused bytes.
     */
    uint16_t unused;

    /**
     * The size of the resource.
     */
    uint32_t resource_size;
};

/**
 * Used to setup handlers for entries.
 */
struct cnwn_ERFHandlers_s {

    /**
     * A handler, can be NULL to use CNWN_ERF_DEFAULT_HANDLER.
     */
    cnwn_ERFHandler handler;
    
    /**
     * Type specific handlers, can be NULL to use the general @ref handler.
     */
    cnwn_ERFHandler type_handler[CNWN_MAX_RESOURCE_TYPE];
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A (default) handler that will simply copy unmodified data as is.
 */
extern CNWN_PUBLIC const cnwn_ERFHandler CNWN_ERF_DEFAULT_HANDLER;

/**
 * Read an ERF file header.
 * @param f The ERF file to read from.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param[out] ret_entries Return an array of entries (must be freed using free()), pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_header(cnwn_File * f, cnwn_ERFHeader * ret_header, cnwn_ERFEntry ** ret_entries);

/**
 * List the contents of an ERF file and print it to a stream.
 * @param path The path to the ERF file.
 * @param details True to print extra details for each entry in the ERF.
 * @param output The output stream, NULL for no output.
 * @returns The number of printed lines or negative on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_list(const char * path, bool details, FILE * output);

/**
 * List the contents of an ERF file and print it to a stream.
 * @param path The path to the ERF file.
 * @param details True to print extra details for each entry in the ERF.
 * @param output The output stream, NULL for no output.
 * @returns The number of printed lines or negative on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_extract(const char * path, bool details, FILE * output);


#ifdef __cplusplus
}
#endif

#endif
