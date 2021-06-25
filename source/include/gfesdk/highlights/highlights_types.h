/* Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

/*!
* \file
* Highlights types
*/

#ifndef HIGHLIGHTS_TYPES_H
#define HIGHLIGHTS_TYPES_H

#include <gfesdk/sdk_types.h>
#include <stdint.h>
#include <stdbool.h>

/*! Possible highlights that can occur during gameplay */
typedef struct
{
    char const*                     id;             //!< Unique id for game event. Max length @ref NVGSDK_MAX_LENGTH - 1
    bool                            userInterest;   /*!< Boolean, whether the player is interested in this event.
                                                    *   User can change this setting. This value provides the default. */
    NVGSDK_HighlightType            highlightTags;  //!< Tags for this highlight
    NVGSDK_HighlightSignificance    significance;   //!< How significant the highlight is
    NVGSDK_LocalizedPair*           nameTable;      /*!< An array of locale-highlightName pairs for the user-facing highlight name.
                                                    *   If no names are given, the highlight id will be used as the "name" */
    size_t                          nameTableSize;  //!< Number of entries in the localeNameTable */
} NVGSDK_Highlight;

/*! Highlight configuration parameters */
typedef struct
{
    NVGSDK_Highlight*   highlightDefinitionTable;
    size_t              highlightTableSize;
    char const*         defaultLocale;              //!< Default locality, used for hinting GFE which translation to use. Defaults to en-us
} NVGSDK_HighlightConfigParams;

/*! Informs the app of the user's highlight choices */
typedef struct
{
    char const* id;     //!< Unique id for game event.
    bool enabled;       //!< Boolean for whether the user wants to record this type of highlight
} NVGSDK_HighlightUserSetting;

/*! Parameters passed into OpenGroup calls */
typedef struct
{
    char const*             groupId;                    /*!< Unique group ID used to refer back to the group for adding Highlights,
                                                        *   or for opening the group Summary later. Max length @ref NVGSDK_MAX_LENGTH - 1 */
    NVGSDK_LocalizedPair*   groupDescriptionTable;      /*!< An array of locale-groupDesc pairs for the user-facing group description.
                                                        *   If no names are given, the groupId will be used as the "name" */
    size_t                  groupDescriptionTableSize;  //!< Number of entries in the groupDescriptionTable */
} NVGSDK_HighlightOpenGroupParams;

/*! Parameters passed into CloseGroup calls */
typedef struct
{
    char const* groupId;            //!< Unique group ID that was used to Open a group previously
    bool        destroyHighlights;  /*!< If set to true, deletes the persisted highlights from disk.
                                    *   Highlights that have been moved to gallery are not removed.
                                    * Note:: With the new sidebar-based overlay UX, all Highlights
                                    * are now part of Gallery by default and this flag is ignored */
} NVGSDK_HighlightCloseGroupParams;

/*! Passed into NVGSDK_Highlights_SetScreenshotHighlightAsync */
typedef struct
{
    char const*             groupId;     //!< Required ID of the group this highlight is attached to.
    char const*             highlightId; //!< Refers to unique highlight id in highlight table
} NVGSDK_ScreenshotHighlightParams;

/*! Passed into NVGSDK_Highlights_SetVideoHighlightAsync */
typedef struct
{
    char const*             groupId;      //!< Required ID of the group this highlight is attached to.
    char const*             highlightId;  //!< Refers to unique highlight id in highlight table
    int32_t                 startDelta;   //!< Milliseconds between the time of request, and the start of desired video clip
    int32_t                 endDelta;     //!< Milliseconds between the time of request, and end of desired video clip
} NVGSDK_VideoHighlightParams;

/*! Represents a filtered highlight group */
typedef struct
{
    char const*                     groupId;            //!< Unique ID of the group to open
    NVGSDK_HighlightType            tagsFilter;         //!< If not zero, represents a bitfield of tags to include 
    NVGSDK_HighlightSignificance    significanceFilter; //!< If not zero, represents a bitfield of significance values to include
} NVGSDK_GroupView;

/*! Choose the highlights to display in the group summary */
typedef struct
{
    NVGSDK_GroupView*   groupSummaryTable;       //!< Pass in all the groups to display on screen
    size_t              groupSummaryTableSize;   //!< Number of groups contained in the table pointer
} NVGSDK_SummaryParams;


/////////////// Responses from API calls

/*! Returned data from NVGSDK_Highlights_GetUserSettings */
typedef struct
{
    NVGSDK_HighlightUserSetting*    highlightSettingTable;
    size_t                          highlightSettingTableSize;
} NVGSDK_Highlights_UserSettings;
/*! Type of callback to provide to NVGSDK_Highlights_GetUserSettings */
typedef void(__stdcall* NVGSDK_Highlights_GetUserSettingsCallback)(NVGSDK_RetCode, NVGSDK_Highlights_UserSettings const*, void*);

/*! Returned data from NVGSDK_Highlights_GetNumberOfHighlightsAsync */
typedef struct
{
    uint16_t numberOfHighlights;
} NVGSDK_Highlights_NumberOfHighlights;
/*! Type of callback to provide to NVGSDK_Highlights_GetNumberOfHighlightsAsync */
typedef void(__stdcall* NVGSDK_Highlights_GetNumberOfHighlightsCallback)(NVGSDK_RetCode, NVGSDK_Highlights_NumberOfHighlights const*, void*);

#endif // HIGHLIGHTS_TYPES_H
