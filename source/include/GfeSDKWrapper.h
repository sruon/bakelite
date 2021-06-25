/* Copyright (c) 2018, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <gfesdk/isdk.h>
#include <gfesdk/highlights/ihighlights.h>


typedef struct _GfeSdkWrapper
{
    void(*Init)(char const* gameName, char const* defaultLocale, NVGSDK_Highlight* highlights, size_t numHighlights, char const* targetPath, int targetPid);
    void(*DeInit)();
    void(*OnTick)();
    void(*OnOpenGroup)(char const* groupId);
    void(*OnCloseGroup)(char const* groupId, bool destroy);
    void(*OnSaveScreenshot)(char const* highlightId, char const* groupId);
    void(*OnSaveVideo)(char const* highlightId, char const* groupId, int startDelta, int endDelta);
    void(*OnGetNumHighlights)(char const* groupId, int sigFilter, int tagFilter);
    void(*OnOpenSummary)(char const* groupIds[], size_t numGroups, int sigFilter, int tagFiler);
    void(*OnRequestLanguage)();
    void(*OnRequestUserSettings)();

    wchar_t const* (*GetCurrentPermissionStr)();
    wchar_t const* (*GetLastOverlayEvent)();
    wchar_t const* (*GetLastResult)();
    wchar_t const* (*GetLastQueryResult)();
} GfeSdkWrapper;

void InitGfeSdkWrapper(GfeSdkWrapper* hl);

#ifdef __cplusplus
}
#endif
