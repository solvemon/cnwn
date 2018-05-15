/**
 * @file resource_type.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_TYPE_H
#define CNWN_RESOURCE_TYPE_H

#include "cnwn/file_system.h"

/**
 * Check if the resource type is ERF compatible.
 * @param t The resource type.
 * @returns True or false.
 */
#define CNWN_RESOURCE_TYPE_VALID(t) ((t) > CNWN_RESOURCE_TYPE_INVALID && (t) < CNWN_MAX_RESOURCE_TYPE && CNWN_RESOURCE_TYPE_INFOS[(t)].type > CNWN_RESOURCE_TYPE_INVALID && CNWN_RESOURCE_TYPE_INFOS[(t)].type < CNWN_MAX_RESOURCE_TYPE && CNWN_RESOURCE_TYPE_INFOS[(t)].extension != NULL)

/**
 * Get the resource type info.
 * @param t The resource type.
 * @returns Returns the info struct.
 */
#define CNWN_RESOURCE_TYPE_INFO(t) (CNWN_RESOURCE_TYPE_VALID((t)) ? CNWN_RESOURCE_TYPE_INFOS[(t)] : CNWN_RESOURCE_TYPE_INFO_INVALID)

/**
 * Check if the resource type is ERF compatible.
 * @param t The resource type.
 * @returns True or false.
 */
#define CNWN_RESOURCE_TYPE_IS_ERF(t) ((t) == CNWN_RESOURCE_TYPE_ERF || (t) == CNWN_RESOURCE_TYPE_HAK || (t) == CNWN_RESOURCE_TYPE_MOD || (t) == CNWN_RESOURCE_TYPE_NWM)

/**
 * Check if the resource type is a container.
 * @param t The resource type.
 * @returns True or false.
 */
#define CNWN_RESOURCE_TYPE_IS_CONTAINER(t) (CNWN_RESOURCE_TYPE_IS_ERF(t) || (t) == CNWN_RESOURCE_TYPE_BIF)

/**
 * Resource types.
 */
enum cnwn_ResourceType_e {

    /**
     * Invalid or unknown.
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
 * Info about a specific resource type.
 */
struct cnwn_ResourceTypeInfo_s {

    /**
     * The type.
     */
    cnwn_ResourceType type;

    /**
     * The file extension.
     */
    const char * extension;

    /**
     * A human readable name.
     */
    const char * name;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Resource info.
 */
extern CNWN_PUBLIC const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFOS[CNWN_MAX_RESOURCE_TYPE];

/**
 * An invalid resource type to return from CNWN_RESOURCE_TYPE_INFO() macro.
 */
extern CNWN_PUBLIC const cnwn_ResourceTypeInfo CNWN_RESOURCE_TYPE_INFO_INVALID;

/**
 * Get the resource type from a path.
 * @param path The path.
 * @returns The resource type or CNWN_RESOURCE_TYPE_INVALID if the filename extension was unavailable or not recognized.
 * @note Only the file extension bit of @p path will be examined, if it contains multiple extensions such as "uti.xml.tar.gz" the first one (uti in this case) will be used.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_path(const char * path);

/**
 * Get the resource type info from a path.
 * @param path The path.
 * @returns The resource type info, may return an invalid info (check info->type).
 * @note Only the file extension bit of @p path will be examined, if it contains multiple extensions such as "uti.xml.tar.gz" the first one (uti in this case) will be used.
 */
extern CNWN_PUBLIC cnwn_ResourceTypeInfo cnwn_resource_type_info_from_path(const char * path);

#ifdef __cplusplus
}
#endif

#endif
