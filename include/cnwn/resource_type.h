/**
 * @file resource_type.h
 * Part of cnwn: Small C99 library and tools for Neverwinter Nights.
 */
#ifndef CNWN_RESOURCE_TYPE_H
#define CNWN_RESOURCE_TYPE_H

#include "cnwn/common.h"
#include "cnwn/file_system.h"

/**
 * Check if a resource type is valid.
 * @param ct The resource type enum.
 * @returns True or false, you know.
 */
#define CNWN_RESOURCE_TYPE_VALID(ct) ((ct) > CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE)

/**
 * Get the extension for a resource type.
 * @param ct The resource type enum.
 * @returns A pointer to a string describing the enumeration extension or an empty string if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_EXTENSION(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_DEFINED_RESOURCES[(ct)].extension : "")

/**
 * Get human readable version of the ERF type enumeration.
 * @param ct The resource type enum.
 * @returns A pointer to a string describing the enumeration name or an empty string if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_NAME(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_DEFINED_RESOURCES[(ct)].name : "")

/**
 * Get the ERF type for a resource type.
 * @param ct The resource type enum.
 * @returns An ERF type or -1 if the enum is out of range.
 */
#define CNWN_RESOURCE_TYPE_INTERNAL_TYPE(ct) ((ct) >= CNWN_RESOURCE_TYPE_NONE && (ct) < CNWN_MAX_RESOURCE_TYPE ? CNWN_DEFINED_RESOURCES[(ct)].internal_type : -1)

/**
 * Describes an ERF entry type.
 */
enum cnwn_ResourceType_e {

    /**
     * Unknown resource type.
     */
    CNWN_RESOURCE_TYPE_NONE = 0,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RES,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BMP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MVE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TGA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WAV,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_INI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MP3,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MPG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PLH,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TEX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDL,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_THG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FNT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LUA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SLT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NSS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NCS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MOD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ARE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SET,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFO,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WOK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_2DA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TLK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TXI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTI,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DLG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ITP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DDS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTR,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GFF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FAC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTE,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTD,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DFT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GIC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GUI,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CSS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CCS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_DWK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BTG,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JRL,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SAV,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTW,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_4PC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SSF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_HAK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NWM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_NDB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTM,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BAK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_OSC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_USC,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRN,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UTR,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UEN,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ULT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SEF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_CAM,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_UPE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ROS,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_RST,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PFB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ZIP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WMP,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BBX,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TFX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_WLK,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_XML,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SCC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PTX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_LTX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_TRX,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDB,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_MDA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_SPT,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_GR2,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXA,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_FXE,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_JPG,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_PWC,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_IDS,
    
    /**
     * 
     */
    CNWN_RESOURCE_TYPE_ERF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_BIF,

    /**
     * 
     */
    CNWN_RESOURCE_TYPE_KEY,
      
    /**
     * Max enum.
     */
    CNWN_MAX_RESOURCE_TYPE
};

/**
 * @see enum cnwn_ResourceType_e
 */
typedef enum cnwn_ResourceType_e cnwn_ResourceType;

/**
 * @see struct cnwn_ResourceDefinition_s
 */
typedef struct cnwn_ResourceDefinition_s cnwn_ResourceDefinition;

/**
 * An resource.
 */
struct cnwn_ResourceDefinition_s {

    /**
     * File extension.
     */
    const char * extension;

    /**
     * The internal type.
     */
    int internal_type;
    
    /**
     * Description.
     */
    const char * description;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Defined resources.
 */
extern CNWN_PUBLIC const cnwn_ResourceDefinition CNWN_DEFINED_RESOURCES[CNWN_MAX_RESOURCE_TYPE];

/**
 * Get the resource type from an internal type.
 * @param internal_type The internal type (as used by Aurora).
 * @returns The resource type or CNWN_RESOURCE_TYPE_NONE if there was no match.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_internal_type(int internal_type);

/**
 * Get the resource type from a filename extension.
 * @param path A path to a file or just the extension.
 * @returns The resource type or CNWN_RESOURCE_TYPE_NONE if there was no match.
 */
extern CNWN_PUBLIC cnwn_ResourceType cnwn_resource_type_from_extension(const char * path);

#ifdef __cplusplus
}
#endif

#endif
