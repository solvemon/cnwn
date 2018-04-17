/**
 * @file erf.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/resource.h"

/**
 * The maximum size of an ERF entry key (including zero terminator).
 * @note The largest key allowed for an ERF entry is 16 (V1.0) and 32 (V1.1) bytes.
 */
#define CNWN_ERF_ENTRY_KEY_MAX_SIZE 33

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
 * A handler for writing files from/to the ERF file.
 * @param handlers The struct from which this handler was set.
 * @param mode The requested mode of operation for the handler, please respect CNWN_ERF_HANDLER_MODE_NONE.
 * @param path The path of the ERF file.
 * @param fd The file descriptor for the ERF file, ONLY READ/WRITE OPERATIONS PERMITTED (no seeking etc).
 * @param output_path The path of the output file.
 * @param output_fd The file descriptor for the output file.
 * @param header The header.
 * @param entry The entry.
 * @returns The number of written bytes or a negative value on error (MUST call cnwn_set_error()).
 * @note If a handler returns a negative value further processing of handlers is discontinued by the calling function.
 */
typedef int64_t (*cnwn_ERFHandler)(const cnwn_ERFHandlers * handlers, cnwn_ERFHandlerMode mode, const char * path, int fd, const char * output_path, int output_fd, const cnwn_ERFHeader * header, const cnwn_ERFEntry * entry);

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
     * The key (name) of the resource.
     */
    char key[CNWN_ERF_ENTRY_KEY_MAX_SIZE];

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
 * Read the header and entries from an ERF file.
 * @param fd The file to read from.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param extended True for extended regular expression, false for POSIX.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Write entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_contents(int fd, const char * regexps[], bool extended, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Read the header and entries from an ERF file.
 * @param path The path to the ERF file.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param extended True for extended regular expression, false for POSIX.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Write entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_contents_path(const char * path, const char * regexps[], bool extended, cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Extract entries from an ERF file to an (optional) output path.
 * @param path The path to the ERF file.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param extended True for extended regular expression, false for POSIX.
 * @param output_dir The output directory (will be created if it doesn't exist), NULL or empty for current working directory.
 * @param handlers The handler setup, pass NULL to use CNWN_ERF_DEFAULT_HANDLER for everything.
 * @param verbose_output A file stream that can be used to write verbose output to, can be NULL for no output.
 * @param[out] ret_bytes The total number of extraced/archived bytes by the handlers, NULL to omit.
 * @param[out] ret_bytes The total number of extraced/archived files by the handlers, NULL to omit.
 * @returns The number of extracted/archived entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_extract(const char * path, const char * regexps[], bool extended, const char * output_dir, const cnwn_ERFHandlers * handlers, FILE * verbose_output, int * ret_files, int64_t * ret_bytes);

#ifdef __cplusplus
}
#endif

#endif
