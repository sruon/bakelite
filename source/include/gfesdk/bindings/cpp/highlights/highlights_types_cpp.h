/* Copyright (c) 2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#ifndef HIGHLIGHTS_TYPES_CPP_H
#define HIGHLIGHTS_TYPES_CPP_H

#include <gfesdk/bindings/cpp/sdk_types_cpp.h>

// Avoid polluting global namespace with C api
namespace GfeSDK
{
#include <gfesdk/highlights/highlights_types.h>
}

namespace GfeSDK
{

//! C++ binding for @ref NVGSDK_Highlight
struct HighlightDefinition
{
    std::string                         id;
    bool                                userDefaultInterest;
    NVGSDK_HighlightType                highlightTags;
    NVGSDK_HighlightSignificance        significance;
    TTranslationTable                   nameLocaleTable;  //!< Locale/translation pairs
    HighlightDefinition() : highlightTags(NVGSDK_HIGHLIGHT_TYPE_NONE), significance(NVGSDK_HIGHLIGHT_SIGNIFICANCE_NONE), userDefaultInterest(true) {}
};

//! C++ binding for @ref NVGSDK_HighlightConfigParams
struct HighlightConfigParams
{
    std::vector<HighlightDefinition> highlightDefinitions;
    std::string defaultLocale;
};

//! C++ binding for @ref NVGSDK_HighlightOpenGroupParams
struct HighlightOpenGroupParams
{
    std::string groupId;
    TTranslationTable groupDescriptionLocaleTable;
};

//! C++ binding for @ref NVGSDK_HighlightCloseGroupParams
struct HighlightCloseGroupParams
{
    std::string groupId;
    bool destroyHighlights;
};

//! C++ binding for @ref NVGSDK_ScreenshotHighlightParams
struct ScreenshotHighlightParams
{
    std::string groupId;
    std::string highlightId;
};

//! C++ binding for @ref NVGSDK_VideoHighlightParams
struct VideoHighlightParams
{
    std::string groupId;
    std::string highlightId;
    int32_t startDelta;
    int32_t endDelta;
};

//! C++ binding for @ref NVGSDK_GroupView
struct GroupView
{
    std::string                     groupId;
    NVGSDK_HighlightType            tagsFilter;
    NVGSDK_HighlightSignificance    significanceFilter;
    GroupView() : tagsFilter(NVGSDK_HIGHLIGHT_TYPE_NONE), significanceFilter(NVGSDK_HIGHLIGHT_SIGNIFICANCE_NONE) {}
};

//! C++ binding for @ref NVGSDK_SummaryParams
struct SummaryParams
{
    std::vector<GroupView>  groupViews;
};

//! C++ binding for @ref NVGSDK_HighlightUserSetting
struct HighlightUserSetting
{
    std::string highlightId;    //!< Unique id for game event.
    bool enabled;               //!< Boolean for whether the user wants to record this type of highlight
};

/////////////// Responses from API calls

//! C++ binding for @ref NVGSDK_GetNumberOfHighlightsResponse
struct GetNumberOfHighlightsResponse
{
    uint16_t numHighlights;
};
typedef std::function<void(NVGSDK_RetCode rc, GetNumberOfHighlightsResponse const*, void*)> TGetNumberOfHighlightsCallback;

//! C++ binding for @ref NVGSDK_Highlights_GetUserSettingsResponse
struct GetUserSettingsResponse
{
    std::vector<HighlightUserSetting> highlightSettings;
};
typedef std::function<void(NVGSDK_RetCode rc, GetUserSettingsResponse const*, void*)> TGetUserSettingsCallback;

}

#endif //HIGHLIGHTS_TYPES_CPP_H
