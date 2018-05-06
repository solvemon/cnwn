/**
 * @file erf.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_ERF_H
#define CNWN_ERF_H

#include "cnwn/file_system.h"
#include "cnwn/resource_type.h"

/**
 * Different types of ERF's.
 */
enum cnwn_ERFType_e {

    /**
     * Invalid.
     */
    CNWN_ERF_TYPE_NONE = 0,

    /**
     * ERF is ERF!
     */
    CNWN_ERF_TYPE_ERF,

    /**
     * Module.
     */
    CNWN_ERF_TYPE_MODULE,
    
    /**
     * Maxe num.
     */
    CNWN_MAX_ERF_TYPE
};

/**
 * @see enum cnwn_ERFType_e
 */
typedef enum cnwn_ERFType_e cnwn_ERFType;

/**
 * @see struct cnwn_ERFEntry_s
 */
typedef struct cnwn_ERFEntry_s cnwn_ERFEntry;

/**
 * @see struct cnwn_ERFInfo_s
 */
typedef struct cnwn_ERFInfo_s cnwn_ERFInfo;

/**
 * An entry in the ERF.
 */
struct cnwn_ERFEntry_s {

    /**
     * The key of the entry.
     */
    char * key;

    /**
     * The resource type of the entry.
     */
    cnwn_ResourceType resource_type;

    /**
     * The offset in the file.
     */
    int64_t resource_offset;

    /**
     * The size of the resource.
     */
    int64_t resource_size;
};

/**
 * Represents an ERF file.
 */
struct cnwn_ERFInfo_s {

    /**
     * Type.
     */
    cnwn_ERFType type;

    /**
     * Major version.
     */
    int major_version;
    
    /**
     * Minor version.
     */
    int minor_version;

    /**
     * The number of localized strings.
     */
    int num_localized_strings;

    /**
     * The localized strings.
     */
    cnwn_LocalizedString * localized_strings;

    /**
     * The number of entries.
     */
    int num_entries;

    /**
     * The entries.
     */
    cnwn_ERFEntry * entries;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read an ERF
 */
extern CNWN_PUBLIC cnwn_ERF * cnwn_erf_read(cnwn_File * f);


#ifdef __cplusplus
}
#endif

#endif
