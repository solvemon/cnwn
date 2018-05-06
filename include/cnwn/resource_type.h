/**
 * @file resource_type.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_TYPE_H
#define CNWN_RESOURCE_TYPE_H

#include "cnwn/file_system.h"

/**
 * Resource types.
 */
enum cnwn_ResourceType_e {

    /**
     * Invalid.
     */
    CNWN_RESOURCE_TYPE_INVALID = -1,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RES = 0,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BMP = 1,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MVE = 2,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TGA = 3,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WAV = 4,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WFX = 5,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLT = 6,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_INI = 7,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MP3 = 8,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MPG = 9,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXT = 10,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLH = 2000,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TEX = 2001,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDL = 2002,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_THG = 2003,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FNT = 2005,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LUA = 2007,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SLT = 2008,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NSS = 2009,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NCS = 2010,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MOD = 2011,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ARE = 2012,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SET = 2013,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFO = 2014,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIC = 2015,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WOK = 2016,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_2DA = 2017,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TLK = 2018,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXI = 2022,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIT = 2023,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTI = 2024,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTI = 2025,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTC = 2026,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTC = 2027,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DLG = 2029,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ITP = 2030,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTT = 2031,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTT = 2032,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DDS = 2033,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTS = 2034,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTS = 2035,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTR = 2036,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GFF = 2037,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FAC = 2038,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTE = 2039,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTE = 2040,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTD = 2041,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTD = 2042,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTP = 2043,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTP = 2044,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DFT = 2045,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIC = 2046,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GUI = 2047,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CSS = 2048,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CCS = 2049,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTM = 2050,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTM = 2051,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DWK = 2052,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWK = 2053,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTG = 2054,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTG = 2055,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JRL = 2056,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SAV = 2057,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTW = 2058,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_4PC = 2059,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SSF = 2060,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_HAK = 2061,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NWM = 2062,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIK = 2063,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NDB = 2064,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTM = 2065,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTT = 2066,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BAK = 2067,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_OSC = 3000,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_USC = 3001,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRN = 3002,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTR = 3003,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UEN = 3004,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ULT = 3005,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SEF = 3006,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFX = 3007,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CAM = 3008,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LFX = 3009,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BFX = 3010,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UPE = 3011,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ROS = 3012,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RST = 3013,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFX = 3014,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFB = 3015,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ZIP = 3016,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WMP = 3017,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BBX = 3018,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TFX = 3019,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WLK = 3020,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_XML = 3021,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SCC = 3022,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTX = 3033,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTX = 3034,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRX = 3035,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDB = 4000,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDA = 4001,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SPT = 4002,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GR2 = 4003,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXA = 4004,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXE = 4005,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JPG = 4007,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWC = 4008,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IDS = 9996,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ERF = 9997,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIF = 9998,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_KEY = 9999,
    
    /**
     * Max enum.
     */
    CNWN_MAX_RESOURCE_TYPE = 10000,
};

/**
 * @see enum cnwn_ResourceType_e
 */
typedef enum cnwn_ResourceType_e cnwn_ResourceType;

/**
 * @see struct cnwn_ResourceTypeInfo_s
 */
typedef struct cnwn_ResourceTypeInfo_s cnwn_ResourceTypeInfo;

/**
 * @see struct cnwn_LocalizedString_s
 */
typedef struct cnwn_LocalizedString_s cnwn_LocalizedString;

/**
 * Info about a specific resource type.
 */
struct cnwn_ResourceTypeInfo_s {

    /**
     * The file extension.
     */
    const char * extension;

    /**
     * The type.
     */
    cnwn_ResourceType type;

    /**
     * A human readable name.
     */
    const char * name;
};

/**
 * Used for localized strings.
 */
struct cnwn_LocalizedString_s {

    /**
     * Language ID.
     */
    int32_t language_id;

    /**
     * The string itself.
     */
    char * text;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Resource info.
 */
extern CNWN_PUBLIC const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFOS[];

/**
 * Check if a resource name is valid or not.
 * @param name The resource name.
 * @returns True or false.
 *
 * A valid name: max 16 characters, only ascii alpha, digits and underscore allowed.
 */
extern CNWN_PUBLIC bool cnwn_resource_name_valid(const char * name);

/**
 * Get the resource info for a type.
 * @param resource_type The resource type.
 * @returns The info (invalid types will return empty info).
 */
extern CNWN_PUBLIC const cnwn_ResourceTypeInfo * cnwn_resource_type_info(cnwn_ResourceType resource_type);

/**
 * Get the resource type from a path.
 * @param path The path.
 * @returns The resource type or CNWN_RESOURCE_TYPE_INVALID if the filename extension was unavailable or not recognized.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_path(const char * path);

/**
 * Initialize a localized string.
 * @param localized_string The struct to initialize.
 * @param language_id The language ID.
 * @param text The string.
 */
extern CNWN_PUBLIC void cnwn_localized_string_init(cnwn_LocalizedString * localized_string, int language_id, const char * text);

/**
 * Read and initialize the localized string from a file.
 * @param localized_string The localized string.
 * @param f The file, must be at the correct offset.
 * @returns The number of read bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 * @note Any previous content in @p localized_string will be discarded.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_init_from_file(cnwn_LocalizedString * localized_string, cnwn_File * f);

/**
 * Deinitialize a localized string.
 * @param localized_string The localized string to initialize.
 */
extern CNWN_PUBLIC void cnwn_localized_string_deinit(cnwn_LocalizedString * localized_string);

/**
 * Write the localized string to a file.
 * @param localized_string The localized string.
 * @param f The file, must be at the correct offset.
 * @returns The number of written bytes or a negative value on error.
 * @see cnwn_get_error() if this function returns a negative value.
 */
extern CNWN_PUBLIC int64_t cnwn_localized_string_write(const cnwn_LocalizedString * localized_string, cnwn_File * f);

#ifdef __cplusplus
}
#endif

#endif
