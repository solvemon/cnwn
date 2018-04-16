/**
 * @file erf.h
 * Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/resource.h"

/**
 * @see struct cnwn_ERFHeader_e
 */
typedef struct cnwn_ERFHeader_s cnwn_ERFHeader;

/**
 * @see struct cnwn_ERFEntry_e
 */
typedef struct cnwn_ERFEntry_s cnwn_ERFEntry;

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
 * Read the header and entries from an ERF file.
 * @param fd The file to read from.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Return entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_contents(int fd, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Read the header and entries from an ERF file.
 * @param path A path to the ERF file.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Return entries here, pass NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_contents_path(const char * path, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries);

/**
 * Read the header and entries from an ERF file.
 * @param path A path to the ERF file.
 * @param regexps A NULL-sentineled array of regexps (OR'ed) to match, NULL to match all.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param max_entries The maximum number of entries to return, zero or a negative value while @p ret_entries is NULL will disable the limit.
 * @param[out] ret_entries Return entries here, pass NULL to omit.
 * @param[out] ret_fd Return the file that was opened from @p path (you must close it manually, unless an error is returned), NULL to omit (in which case the file is closed automatically).
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error (in which case the file is closed automatically).
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_contents_path_fd(const char * path, const char * regexps[], cnwn_ERFHeader * ret_header, int max_entries, cnwn_ERFEntry * ret_entries, int * ret_fd);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_fd The output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract(int fd, uint32_t offset, uint32_t size, int output_fd);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_fd The output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract(int fd, uint32_t offset, uint32_t size, int output_fd);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param fd The file to read the entry from.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_path A path to the output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract_path(int fd, uint32_t offset, uint32_t size, const char * output_path);

/**
 * Extract a single entry from an ERF file and write to output file.
 * @param path A path to the ERF file.
 * @param offset The offset where the entry can be found in @p fd.
 * @param size The size of the entry to write.
 * @param output_path A path to the output file.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_entry_extract_path2(const char * path, uint32_t offset, uint32_t size, const char * output_path);


#ifdef __cplusplus
}
#endif

#endif
