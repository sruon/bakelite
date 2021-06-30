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

#include <gfesdk/highlights/ihighlights.h>
#include <gfesdk/isdk.h>
// Core

typedef NVGSDK_RetCode(__cdecl* NVGSDK_Createfn)(
    NVGSDK_HANDLE**,
    NVGSDK_CreateInputParams const*,
    NVGSDK_CreateResponse*);

typedef NVGSDK_RetCode(__cdecl* NVGSDK_Releasefn)(NVGSDK_HANDLE*);
typedef NVGSDK_RetCode(__cdecl* NVGSDK_Pollfn)(NVGSDK_HANDLE*);
typedef NVGSDK_RetCode(__cdecl* NVGSDK_SetLogLevelfn)(NVGSDK_LogLevel);
typedef NVGSDK_RetCode(__cdecl* NVGSDK_AttachLogListenerfn)(
    NVGSDK_LoggingCallback);

typedef NVGSDK_RetCode(__cdecl* NVGSDK_SetListenerLogLevelfn)(
    NVGSDK_LogLevel);
typedef void(__cdecl* NVGSDK_RequestPermissionsAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_RequestPermissionsParams const*,
    NVGSDK_EmptyCallback,
    void*);
typedef void(__cdecl* NVGSDK_GetUILanguageAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_GetUILanguageCallback,
    void*);

// Highlights
typedef void(__cdecl* NVGSDK_Highlights_ConfigureAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_HighlightConfigParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_GetUserSettingsAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_Highlights_GetUserSettingsCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_OpenGroupAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_HighlightOpenGroupParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_CloseGroupAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_HighlightCloseGroupParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_SetScreenshotHighlightAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_ScreenshotHighlightParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_SetVideoHighlightAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_VideoHighlightParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_OpenSummaryAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_SummaryParams const*,
    NVGSDK_EmptyCallback,
    void*);

typedef void(__cdecl* NVGSDK_Highlights_GetNumberOfHighlightsAsyncfn)(
    NVGSDK_HANDLE*,
    NVGSDK_GroupView const*,
    NVGSDK_Highlights_GetNumberOfHighlightsCallback,
    void*);

NVGSDK_Createfn NVGSDK_Create;
NVGSDK_Releasefn NVGSDK_Release;
NVGSDK_Pollfn NVGSDK_Poll;
NVGSDK_SetLogLevelfn NVGSDK_SetLogLevel;
NVGSDK_AttachLogListenerfn NVGSDK_AttachLogListener;
NVGSDK_SetListenerLogLevelfn NVGSDK_SetListenerLogLevel;
NVGSDK_RequestPermissionsAsyncfn NVGSDK_RequestPermissionsAsync;
NVGSDK_GetUILanguageAsyncfn NVGSDK_GetUILanguageAsync;
NVGSDK_Highlights_ConfigureAsyncfn NVGSDK_Highlights_ConfigureAsync;

NVGSDK_Highlights_GetUserSettingsAsyncfn NVGSDK_Highlights_GetUserSettingsAsync;
NVGSDK_Highlights_OpenGroupAsyncfn NVGSDK_Highlights_OpenGroupAsync;
NVGSDK_Highlights_CloseGroupAsyncfn NVGSDK_Highlights_CloseGroupAsync;
NVGSDK_Highlights_SetScreenshotHighlightAsyncfn
    NVGSDK_Highlights_SetScreenshotHighlightAsync;
NVGSDK_Highlights_SetVideoHighlightAsyncfn
    NVGSDK_Highlights_SetVideoHighlightAsync;
NVGSDK_Highlights_OpenSummaryAsyncfn NVGSDK_Highlights_OpenSummaryAsync;
NVGSDK_Highlights_GetNumberOfHighlightsAsyncfn
    NVGSDK_Highlights_GetNumberOfHighlightsAsync;

typedef struct _GfeSdkWrapper {
  void (*Init)(char const* gameName,
               char const* defaultLocale,
               NVGSDK_Highlight* highlights,
               size_t numHighlights,
               char const* targetPath,
               int targetPid);
  void (*DeInit)();
  void (*OnTick)();
  void (*OnOpenGroup)(char const* groupId);
  void (*OnCloseGroup)(char const* groupId, bool destroy);
  void (*OnSaveScreenshot)(char const* highlightId, char const* groupId);
  void (*OnSaveVideo)(char const* highlightId,
                      char const* groupId,
                      int startDelta,
                      int endDelta);
  void (*OnGetNumHighlights)(char const* groupId, int sigFilter, int tagFilter);
  void (*OnOpenSummary)(char const* groupIds[],
                        size_t numGroups,
                        int sigFilter,
                        int tagFiler);
  void (*OnRequestLanguage)();
  void (*OnRequestUserSettings)();

  wchar_t const* (*GetCurrentPermissionStr)();
  wchar_t const* (*GetLastOverlayEvent)();
  wchar_t const* (*GetLastResult)();
  wchar_t const* (*GetLastQueryResult)();
} GfeSdkWrapper;

void InitGfeSdkWrapper(GfeSdkWrapper* hl);

#ifdef __cplusplus
}
#endif
