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
* C++ bindings for GfeSDK Highlights.
* Implemented in the header to avoid ABI incompatibilities
*/

#ifndef GFESDK_IHIGHLIGHTS_CPP_IMPL_H
#define GFESDK_IHIGHLIGHTS_CPP_IMPL_H

#include "ihighlights_cpp.h"
#include <gfesdk/bindings/cpp/isdk_cpp_impl.h>

#include <memory>

namespace GfeSDK
{
class HighlightsImpl : public Highlights
{
public:
    /*! C++ binding for \ref NVGSDK_Highlights_ConfigureAsync */
    virtual void ConfigureAsync(HighlightConfigParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_GetUserSettingsAsync */
    virtual void GetUserSettingsAsync(TGetUserSettingsCallback cb = nullptr, void* cbContext = nullptr) const;
    /*! C++ binding for \ref NVGSDK_Highlights_OpenGroupAsync */
    virtual void OpenGroupAsync(HighlightOpenGroupParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_CloseGroupAsync */
    virtual void CloseGroupAsync(HighlightCloseGroupParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_SetScreenshotHighlightAsync */
    virtual void SetScreenshotHighlightAsync(ScreenshotHighlightParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_SetVideoHighlightAsync */
    virtual void SetVideoHighlightAsync(VideoHighlightParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_OpenSummaryAsync */
    virtual void OpenSummaryAsync(SummaryParams const& params, TCallback cb = nullptr, void* cbContext = nullptr);
    /*! C++ binding for \ref NVGSDK_Highlights_GetNumberOfHighlightsAsync */
    virtual void GetNumberOfHighlightsAsync(GroupView const& params, TGetNumberOfHighlightsCallback cb = nullptr, void* cbContext = nullptr);

    friend class Highlights;
private:
    // Remove default construct, copy and assignment operators
    HighlightsImpl(Core* core) : m_handle(core ? core->GetHandle() : nullptr) {}
    HighlightsImpl(const HighlightsImpl& other) {}
    HighlightsImpl& operator=(const HighlightsImpl& other) {}

private:
    NVGSDK_HANDLE* m_handle;
};

inline Highlights* Highlights::Create(Core* core)
{
    return new HighlightsImpl(core);
}

inline void HighlightsImpl::ConfigureAsync(HighlightConfigParams const& params, TCallback callback, void* context)
{
    NVGSDK_HighlightConfigParams cParams;
    std::vector<NVGSDK_Highlight> cHighlights;
    std::vector<std::vector<NVGSDK_LocalizedPair>> cLocaleMaps;
    for (auto it = params.highlightDefinitions.begin(); it != params.highlightDefinitions.end(); ++it)
    {
        cLocaleMaps.push_back(std::vector<NVGSDK_LocalizedPair>());
        for (auto localeIt = it->nameLocaleTable.begin(); localeIt != it->nameLocaleTable.end(); ++localeIt)
        {
            cLocaleMaps.back().push_back({
                localeIt->first.c_str(),
                localeIt->second.c_str()
            });
        }
        cHighlights.push_back({
            it->id.c_str(),
            it->userDefaultInterest,
            it->highlightTags,
            it->significance,
            it->nameLocaleTable.empty() ? nullptr : &cLocaleMaps.back()[0],
            it->nameLocaleTable.size()
        });
    }
    cParams.highlightDefinitionTable = &cHighlights[0];
    cParams.highlightTableSize = cHighlights.size();
    cParams.defaultLocale = params.defaultLocale.c_str();
    NVGSDK_Highlights_ConfigureAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::GetUserSettingsAsync(TGetUserSettingsCallback callback, void* context) const
{
    NVGSDK_Highlights_GetUserSettingsAsync(m_handle, [](NVGSDK_RetCode rc, NVGSDK_Highlights_UserSettings const* data, void* context) {
        std::unique_ptr<CoreImpl::CallbackContext<TGetUserSettingsCallback>> callbackContext(reinterpret_cast<CoreImpl::CallbackContext<TGetUserSettingsCallback>*>(context));
        if (!callbackContext->callback) return;

        if (NVGSDK_FAILED(rc))
        {
            return callbackContext->callback(rc, nullptr, callbackContext->appContext);
        }

        GetUserSettingsResponse r;
        for (size_t i = 0; i < data->highlightSettingTableSize; ++i)
        {
            r.highlightSettings.push_back({
                data->highlightSettingTable[i].id,
                data->highlightSettingTable[i].enabled
            });
        }
        callbackContext->callback(rc, &r, callbackContext->appContext);
    }, new CoreImpl::CallbackContext<TGetUserSettingsCallback>({ callback, context }));
}

inline void HighlightsImpl::OpenGroupAsync(HighlightOpenGroupParams const& params, TCallback callback, void* context)
{
    NVGSDK_HighlightOpenGroupParams cParams;

    std::vector<NVGSDK_LocalizedPair> localePairs;
    CoreImpl::translateLocaleTable(params.groupDescriptionLocaleTable, localePairs);
    cParams.groupId = params.groupId.c_str();
    cParams.groupDescriptionTable = &localePairs[0];
    cParams.groupDescriptionTableSize = localePairs.size();

    NVGSDK_Highlights_OpenGroupAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::CloseGroupAsync(HighlightCloseGroupParams const& params, TCallback callback, void* context)
{
    NVGSDK_HighlightCloseGroupParams cParams;
    cParams.groupId = params.groupId.c_str();
    cParams.destroyHighlights = params.destroyHighlights;

    NVGSDK_Highlights_CloseGroupAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::SetScreenshotHighlightAsync(ScreenshotHighlightParams const& params, TCallback callback, void* context)
{
    NVGSDK_ScreenshotHighlightParams cParams;
    cParams.groupId = params.groupId.c_str();
    cParams.highlightId = params.highlightId.c_str();
    NVGSDK_Highlights_SetScreenshotHighlightAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::SetVideoHighlightAsync(VideoHighlightParams const& params, TCallback callback, void* context)
{
    NVGSDK_VideoHighlightParams cParams;
    cParams.groupId = params.groupId.c_str();
    cParams.highlightId = params.highlightId.c_str();
    cParams.startDelta = params.startDelta;
    cParams.endDelta = params.endDelta;
    NVGSDK_Highlights_SetVideoHighlightAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::OpenSummaryAsync(SummaryParams const& params, TCallback callback, void* context)
{
    std::vector<NVGSDK_GroupView> cGroupViews;
    for (auto it = params.groupViews.begin(); it != params.groupViews.end(); ++it)
    {
        cGroupViews.push_back({
            it->groupId.c_str(),
            it->tagsFilter,
            it->significanceFilter
        });
    }

    NVGSDK_SummaryParams cParams;
    cParams.groupSummaryTable = &cGroupViews[0];
    cParams.groupSummaryTableSize = cGroupViews.size();
    NVGSDK_Highlights_OpenSummaryAsync(m_handle, &cParams, CoreImpl::callbackWrapper, new CoreImpl::CallbackContext<TCallback>({ callback, context }));
}

inline void HighlightsImpl::GetNumberOfHighlightsAsync(GroupView const& params, TGetNumberOfHighlightsCallback callback, void* context)
{
    NVGSDK_GroupView cParams;
    cParams.groupId = params.groupId.c_str();
    cParams.tagsFilter = params.tagsFilter;
    cParams.significanceFilter = params.significanceFilter;
    NVGSDK_Highlights_GetNumberOfHighlightsAsync(m_handle, &cParams, [](NVGSDK_RetCode rc, NVGSDK_Highlights_NumberOfHighlights const* data, void* context) {
        std::unique_ptr<CoreImpl::CallbackContext<TGetNumberOfHighlightsCallback>> callbackContext(reinterpret_cast<CoreImpl::CallbackContext<TGetNumberOfHighlightsCallback>*>(context));
        if (!callbackContext->callback) return;

        if (NVGSDK_FAILED(rc))
        {
            return callbackContext->callback(rc, nullptr, callbackContext->appContext);
        }

        GetNumberOfHighlightsResponse num;
        num.numHighlights = data->numberOfHighlights;
        callbackContext->callback(rc, &num, callbackContext->appContext);
    }, new CoreImpl::CallbackContext<TGetNumberOfHighlightsCallback>({ callback, context }));
}

}

#endif //GFESDK_IHIGHLIGHTS_CPP_IMPL_H
