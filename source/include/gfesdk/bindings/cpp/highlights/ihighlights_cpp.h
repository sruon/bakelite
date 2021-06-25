/* Copyright (c) 2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

/*!
* \file
* C++ bindings for GfeSDK.
* Implemented in the header to avoid ABI incompatibilities
*/

#ifndef GFESDK_IHIGHLIGHTS_CPP_H
#define GFESDK_IHIGHLIGHTS_CPP_H

#include <gfesdk/bindings/cpp/isdk_cpp.h>
#include <gfesdk/bindings/cpp/highlights/highlights_types_cpp.h>

namespace GfeSDK
{
// Avoid polluting global namespace with C api, so put it inside namespace
#include <gfesdk/highlights/ihighlights.h>

/*! C++ Highlights interface */
class Highlights
{
public:
    static Highlights* Create(Core* core);
    virtual ~Highlights() {}

    /*! C++ binding for \ref NVGSDK_Highlights_ConfigureAsync */
    virtual void ConfigureAsync(HighlightConfigParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_GetUserSettingsAsync */
    virtual void GetUserSettingsAsync(TGetUserSettingsCallback cb = nullptr, void* cbContext = nullptr) const = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_OpenGroupAsync */
    virtual void OpenGroupAsync(HighlightOpenGroupParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_CloseGroupAsync */
    virtual void CloseGroupAsync(HighlightCloseGroupParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_SetScreenshotHighlightAsync */
    virtual void SetScreenshotHighlightAsync(ScreenshotHighlightParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_SetVideoHighlightAsync */
    virtual void SetVideoHighlightAsync(VideoHighlightParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_OpenSummaryAsync */
    virtual void OpenSummaryAsync(SummaryParams const& params, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_Highlights_GetNumberOfHighlightsAsync */
    virtual void GetNumberOfHighlightsAsync(GroupView const& params, TGetNumberOfHighlightsCallback cb = nullptr, void* cbContext = nullptr) = 0;
};
}

#endif //GFESDK_IHIGHLIGHTS_CPP_H
