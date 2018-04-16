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
 * @see struct cnwn_ERFHeader_e
 */
typedef struct cnwn_ERFHeader_s cnwn_ERFHeader;

/**
 * @see struct cnwn_ERFEntry_e
 */
typedef struct cnwn_ERFEntry_s cnwn_ERFEntry;

/**
 * Called for each entry that requires writing.
 * @param path The path of the ERF file.
 * @param fd The file descriptor for the ERF file, handlers have to seek this fd to find the entry.
 * @param header The header.
 * @param entry The entry.
 * @param output_path An output path for the entry.
 * @param verbose True if the handler should report info to stdout (NOT stderr!).
 * @param force True to omit any questions about overwriting existing files.
 * @param[out] ret_bytes The implementation will return the number of written bytes, NULL should be acceptable to omit the return value.
 * @returns The implementation should return the number of written files or a negative number on error (plz set the error accordingly using cnwn_set_error() instead of printing to stderr).
 * @note It is acceptable for a handler to ignore an entry and simply return 0.
 * @note If a handler returns a negative value further processing of handlers will be discontinued.
 */
typedef int (*cnwn_ERFExtractionHandler)(const char * path, int fd, const cnwn_ERFHeader * header, const cnwn_ERFEntry * entry, const char * output_path, bool verbose, bool force, int64_t * ret_bytes);

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


#ifdef __cplusplus
extern "C" {
#endif

/**
 * A (default) handler that will simply extract the binary format of an entry and save it as a normal file.
 */
extern CNWN_PUBLIC const cnwn_ERFExtractionHandler CNWN_ERF_EXTRACTION_HANDLER_BINARY_COPY;

/**
 * A handler that prints to stdout (if verbose) and return values as if the files would have been copied.
 */
extern CNWN_PUBLIC const cnwn_ERFExtractionHandler CNWN_ERF_EXTRACTION_HANDLER_DRYRUN;

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
 * @param verbose True to print info to stdout, false means no stdout.
 * @param force Don't ask for stdin input, assume YES for all questions.
 * @param exiterr True if the extraction should halt if a handler returns an error, false to simply report them to stderr and continue.
 * @param handler A handler that will be called for each entry, NULL will use @p ref CNWN_ERF_EXTRACTION_HANDLER_BINARY_COPY.
 * @param[out] ret_bytes The total number of written bytes, NULL to omit.
 * @param[out] ret_entries The total number of written files, NULL to omit.
 * @returns Zero on success or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note If @p force is false this function may require input from stdin will hang there until it's received.
 */
extern CNWN_PUBLIC int cnwn_erf_extract(const char * path, const char * regexps[], bool extended, const char * output_dir, bool verbose, bool force, bool exiterr, cnwn_ERFExtractionHandler * handler, int64_t * ret_bytes, int * ret_entries);

#ifdef __cplusplus
}
#endif

#endif
