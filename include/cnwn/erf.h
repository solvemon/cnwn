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
 * Extract an entry from an ERF file.
 * @param input_f The file to read from.
 * @param header The header of the ERF.
 * @param resouce_type The type of entry to extract.
 * @param resource_size The size of the resouce.
 * @param output_f The file to write to.
 * @returns The number of written bytes (to @p output_f) or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int (*cnwn_ErfExtractEntry)(cnwn_File * input_f, const cnwn_ERFHeader * header, cnwn_ResourceType resource_type, int64_t resource_size, cnwn_File * output_f);

/**
 * Archive an entry to an ERF file.
 * @param input_f The file to read from.
 * @param header The header of the ERF.
 * @param resouce_type The type of entry to extract.
 * @param output_f The file to write to.
 * @returns The number of written bytes (to @p f) or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
typedef int (*cnwn_ErfArchiveEntry)(cnwn_File * input_f, const cnwn_ERFHeader * header, cnwn_ResourceType resouce_type, cnwn_File * output_f);


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
     * The size of the entire ERF file.
     */
    int64_t filesize;
    
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
    
    /**
     * The data of the rest of header.
     */
    uint8_t rest[128];
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
    uint16_t itype;
    
    /**
     * Some unused bytes.
     */
    uint16_t unused;

    /**
     * The size of the resource.
     */
    uint32_t resource_size;

    /**
     * At what position the entry was found in the ERF.
     */
    int index;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Get a printable string version of an ERF header.
 * @param header The ERF header.
 * @param detailed True if you want more info, false for a compressed format.
 * @param max_size The maximum size of the returned string (including zero terminator).
 * @param[out] ret_s Return the string here, pass NULL to get the required length.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_erf_header_to_string(const cnwn_ERFHeader * header, bool detailed, int max_size, char * ret_s);

/**
 * Get a printable string version of an ERF entry.
 * @param entry The ERF entry.
 * @param detailed True if you want more info, false for a compressed format.
 * @param max_size The maximum size of the returned string (including zero terminator).
 * @param[out] ret_s Return the string here, pass NULL to get the required length.
 * @returns The length of the return string (excluding zero terminator).
 */
extern CNWN_PUBLIC int cnwn_erf_entry_to_string(const cnwn_ERFEntry * entry, bool detailed, int max_size, char * ret_s);

/**
 * Read an ERF file header.
 * @param f The ERF file to read from.
 * @param[out] ret_header Return the header, NULL to omit.
 * @param[out] ret_entries Return an array of entries (must be freed using free()), pass NULL to omit.
 * @param[out] ret_bytes The number of read bytes, NULL to omit.
 * @returns The number of returned entries (or available entries if @p ret_entries is NULL) limited by @p max_entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_header(cnwn_File * f, cnwn_ERFHeader * ret_header, cnwn_ERFEntry ** ret_entries, int64_t * ret_bytes);

/**
 * Write an ERF file header.
 * @param f The ERF file to write to.
 * @param header The header.
 * @param num_entries The number of entries to write.
 * @param entries The entries.
 * @param[out] ret_bytes The number of written bytes, NULL to omit.
 * @returns The number of written entries or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_write_header(cnwn_File * f, const cnwn_ERFHeader * header, int num_entries, const cnwn_ERFEntry * entries, int64_t * ret_bytes);

/**
 * Read localized strings from an ERF file.
 * @param f The ERF file to read from.
 * @param header The header of the ERF.
 * @param[out] ret_strings Return an array of strings (NULL sentinel, must be freed using cnwn_freestring()) or NULL if no strings were found, pass NULL to omit.
 * @param[out] ret_bytes The number of read bytes, NULL to omit.
 * @returns The number of returned strings (or available entries if @p ret_strings is NULL).
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int cnwn_erf_read_localized_strings(cnwn_File * f, const cnwn_ERFHeader * header, char *** ret_strings, int64_t * ret_bytes);

#ifdef __cplusplus
}
#endif

#endif
