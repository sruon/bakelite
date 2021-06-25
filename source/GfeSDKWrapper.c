/* Copyright (c) 2018, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#include <Windows.h>

#include "GfeSDKWrapper.h"

#include <gfesdk/sdk_types.h>
#include <gfesdk/isdk.h>

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char g_logBuffer[512];
void dbgprint(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vsprintf_s(g_logBuffer, sizeof(g_logBuffer) / sizeof(g_logBuffer[0]), fmt, args);
    printf(g_logBuffer);
    printf("\n");
    OutputDebugStringA(g_logBuffer);
    OutputDebugStringA("\n");
    va_end(args);
}
#define LOG(fmt, ...) dbgprint(fmt, __VA_ARGS__)

#define VALIDATE_HANDLE() \
    if (!g_sdk) {               \
        LOG("Invalid handle!"); \
        return;                 \
    }
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

NVGSDK_HANDLE* g_sdk = NULL;
#define MAX_QUERY_STRING 2000
wchar_t g_lastQueryResult[MAX_QUERY_STRING];
wchar_t g_lastResult[NVGSDK_MAX_LENGTH];
wchar_t g_permissionStr[NVGSDK_MAX_LENGTH];
wchar_t g_overlayStateStr[NVGSDK_MAX_LENGTH];

static void ConfigureHighlights(char const* defaultLocale, NVGSDK_Highlight* highlights, size_t numHighlights);
static void __stdcall handleNotification(NVGSDK_NotificationType type, NVGSDK_Notification const* response, void* context);
static void __stdcall handlePermissionChanged(NVGSDK_ScopePermission* scopePermissionTable, size_t size);
static void __stdcall handleGenericResponse(NVGSDK_RetCode rc, void* context);
static void __stdcall handlePermissionRequested(NVGSDK_RetCode rc, void* context);
static void updateResultString(NVGSDK_RetCode rc);

typedef struct
{
    char const* defaultLocale;
    NVGSDK_Highlight* highlights;
    size_t numHighlights;
} TConfigHolder;

void __stdcall handlePermissionRequested(NVGSDK_RetCode rc, void* context)
{
    updateResultString(rc);
    TConfigHolder* configHolder = context;

    if (NVGSDK_SUCCEEDED(rc))
    {
        ConfigureHighlights(configHolder->defaultLocale, configHolder->highlights, configHolder->numHighlights);
    }

    free(context);
}

void Init(char const* gameName, char const* defaultLocale, NVGSDK_Highlight* highlights, size_t numHighlights, char const* targetPath, int targetPid)
{
    memset(g_lastQueryResult, 0, MAX_QUERY_STRING);
    memset(g_lastResult, 0, NVGSDK_MAX_LENGTH);
    memset(g_permissionStr, 0, NVGSDK_MAX_LENGTH);
    memset(g_overlayStateStr, 0, NVGSDK_MAX_LENGTH);

    //! [Creation C]
    NVGSDK_CreateInputParams inParams;
    memset(&inParams, 0, sizeof(inParams));

    NVGSDK_Scope scopes[] = { NVGSDK_SCOPE_HIGHLIGHTS, NVGSDK_SCOPE_HIGHLIGHTS_VIDEO };
    NVGSDK_ScopePermission scopePermissions[COUNT_OF(scopes)];

    inParams.appName = "Rocket League";
    inParams.pollForCallbacks = true;
    inParams.scopeTable = &scopes[0];
    inParams.scopeTableSize = COUNT_OF(scopes);
    inParams.notificationCallback = handleNotification;

    if (targetPath != NULL && targetPid != 0)
    {
        inParams.targetPath = targetPath;
        inParams.targetPid = targetPid;
    }

    NVGSDK_CreateResponse outParams;
    memset(&outParams, 0, sizeof(outParams));
    outParams.scopePermissionTable = &scopePermissions[0];
    outParams.scopePermissionTableSize = COUNT_OF(scopes);

    NVGSDK_RetCode rc = NVGSDK_Create(&g_sdk, &inParams, &outParams);
    if (NVGSDK_SUCCEEDED(rc))
    {
        // Valid handle has been returned
        LOG("Success: %s", NVGSDK_RetCodeToString(rc));
        LOG("PC is running GFE version %s", outParams.gfeVersionStr);
        LOG("PC is running GfeSDK version %d.%d", outParams.versionMajor, outParams.versionMinor);
        switch (rc)
        {
        case NVGSDK_SUCCESS_VERSION_OLD_GFE:
            LOG("Compatible, but the user should update to the latest version of GFE.");
            break;
        case NVGSDK_SUCCESS_VERSION_OLD_SDK:
            LOG("Compatible, but this application should update to a more recent GfeSDK to get latest features/bugfixes");
            break;
        }
    }
    else
    {
        // No valid handle
        LOG("Failure: %s", NVGSDK_RetCodeToString(rc));
        switch (rc)
        {
        case NVGSDK_ERR_SDK_VERSION:
            LOG("This version of the SDK is too old to communicate with the user's SDK. We're never planning on this happening.");
            LOG("PC is running GFE version %s", outParams.gfeVersionStr);
            LOG("PC is running GfeSDK version %d.%d", outParams.versionMajor, outParams.versionMinor);
            break;
        case NVGSDK_SUCCESS_VERSION_OLD_SDK:
            LOG("The installed version of GFE is too old to continue. User must upgrade.");
            LOG("PC is running GFE version %s", outParams.gfeVersionStr);
            LOG("PC is running GfeSDK version %d.%d", outParams.versionMajor, outParams.versionMinor);
            break;
        }
        return;
    }
    //! [Creation C]

    handlePermissionChanged(outParams.scopePermissionTable, outParams.scopePermissionTableSize);

    //! [Permissions C]
    // Request Permissions if user hasn't decided yet
    NVGSDK_RequestPermissionsParams requestPermissionsParams = { 0 };
    NVGSDK_Scope requestScopes[COUNT_OF(scopes)];
    memset(requestScopes, NVGSDK_SCOPE_MAX, COUNT_OF(scopes));
    requestPermissionsParams.scopeTable = &requestScopes[0];

    // 'response' came from create call. It tells us which permissions we requested during Create,
    // but the user hasn't yet made a decision on
    for (size_t i = 0, j = 0; i < outParams.scopePermissionTableSize; ++i)
    {
        if (outParams.scopePermissionTable[i].permission == NVGSDK_PERMISSION_MUST_ASK)
        {
            requestPermissionsParams.scopeTable[j++] = outParams.scopePermissionTable[i].scope;
            requestPermissionsParams.scopeTableSize++;
        }
    }

    if (requestPermissionsParams.scopeTableSize > 0)
    {
        TConfigHolder* configHolder = malloc(sizeof(TConfigHolder));
        configHolder->defaultLocale = defaultLocale;
        configHolder->highlights = highlights;
        configHolder->numHighlights = numHighlights;

        // If the user hasn't given permission for recording yet, ask them to do so now via overlay
        NVGSDK_RequestPermissionsAsync(g_sdk, &requestPermissionsParams, &handlePermissionRequested, configHolder);
    }
    else
    {
        // Otherwise, go ahead and set up now
        ConfigureHighlights(defaultLocale, highlights, numHighlights);
    }
    //! [Permissions C]
}

void DeInit()
{
    VALIDATE_HANDLE();

    //! [Release C]
    NVGSDK_Release(g_sdk);
    //! [Release C]
}

void OnTick()
{
    VALIDATE_HANDLE();

    NVGSDK_Poll(g_sdk);
}

void OnOpenGroup(char const* groupId)
{
    VALIDATE_HANDLE();

    //! [OpenGroup C]
    NVGSDK_HighlightOpenGroupParams params = { 0 };
    params.groupId = groupId;
    NVGSDK_Highlights_OpenGroupAsync(g_sdk, &params, &handleGenericResponse, NULL);
    //! [OpenGroup C]
}

void OnCloseGroup(char const* groupId, bool destroy)
{
    VALIDATE_HANDLE();

    //! [CloseGroup C]
    NVGSDK_HighlightCloseGroupParams params = { 0 };
    params.groupId = groupId;
    params.destroyHighlights = destroy;
    NVGSDK_Highlights_CloseGroupAsync(g_sdk, &params, &handleGenericResponse, NULL);
    //! [CloseGroup C]
}

void OnSaveScreenshot(char const* highlightId, char const* groupId)
{
    VALIDATE_HANDLE();

    NVGSDK_ScreenshotHighlightParams params;
    params.groupId = groupId;
    params.highlightId = highlightId;
    NVGSDK_Highlights_SetScreenshotHighlightAsync(g_sdk, &params, &handleGenericResponse, NULL);
}

void OnSaveVideo(char const* highlightId, char const* groupId, int startDelta, int endDelta)
{
    VALIDATE_HANDLE();

    //! [SaveVideo C]
    NVGSDK_VideoHighlightParams params;
    params.groupId = groupId;
    params.highlightId = highlightId;
    params.startDelta = startDelta;
    params.endDelta = endDelta;
    NVGSDK_Highlights_SetVideoHighlightAsync(g_sdk, &params, &handleGenericResponse, NULL);
    //! [SaveVideo C]
}

void __stdcall handleSummaryOpened(NVGSDK_RetCode rc, void* context)
{
    updateResultString(rc);
    NVGSDK_SummaryParams* params = context;
    free(params);
}

void OnOpenSummary(char const* groupIds[], size_t numGroups, int sigFilter, int tagFilter)
{
    VALIDATE_HANDLE();

    //! [OpenSummary C]
    NVGSDK_SummaryParams* params = calloc(1, sizeof(NVGSDK_SummaryParams));
    params->groupSummaryTable = calloc(numGroups, sizeof(NVGSDK_GroupView));
    params->groupSummaryTableSize = numGroups;

    for (size_t i = 0; i < numGroups; ++i)
    {
        params->groupSummaryTable[i].groupId = groupIds[i];
        params->groupSummaryTable[i].significanceFilter = sigFilter;
        params->groupSummaryTable[i].tagsFilter = tagFilter;
    }

    NVGSDK_Highlights_OpenSummaryAsync(g_sdk, params, &handleSummaryOpened, params);
    //! [OpenSummary C]
}

void __stdcall handleGotNumHighlights(NVGSDK_RetCode rc, NVGSDK_Highlights_NumberOfHighlights const* response, void* context)
{
    updateResultString(rc);
    if (NVGSDK_SUCCEEDED(rc))
    {
        swprintf(g_lastQueryResult, MAX_QUERY_STRING, L"#: %d", response->numberOfHighlights);
    }
}

void OnGetNumHighlights(char const* groupId, int sigFilter, int tagFilter)
{
    VALIDATE_HANDLE();

    NVGSDK_GroupView groupView;
    groupView.groupId = groupId;
    groupView.significanceFilter = sigFilter;
    groupView.tagsFilter = tagFilter;

    NVGSDK_Highlights_GetNumberOfHighlightsAsync(g_sdk, &groupView, handleGotNumHighlights, NULL);
}

void __stdcall handleGotLanguage(NVGSDK_RetCode rc, NVGSDK_Language const* response, void* context)
{
    updateResultString(rc);
    if (NVGSDK_SUCCEEDED(rc))
    {
        swprintf(g_lastQueryResult, MAX_QUERY_STRING, L"%hs", response->cultureCode);
    }
}

void OnRequestLanguage()
{
    VALIDATE_HANDLE();

    NVGSDK_GetUILanguageAsync(g_sdk, handleGotLanguage, NULL);
}

void __stdcall handleGotUserSettings(NVGSDK_RetCode rc, NVGSDK_Highlights_UserSettings const* response, void* context)
{
    updateResultString(rc);
    if (NVGSDK_SUCCEEDED(rc))
    {
        size_t endOfStr = 0;
        for (size_t i = 0; i < response->highlightSettingTableSize; ++i)
        {
            endOfStr += swprintf(g_lastQueryResult + endOfStr, MAX_QUERY_STRING - endOfStr, L"\n%hs: %hs", response->highlightSettingTable[i].id, response->highlightSettingTable[i].enabled ? "ON" : "OFF");
        }
    }
}

void OnRequestUserSettings()
{
    VALIDATE_HANDLE();

    NVGSDK_Highlights_GetUserSettingsAsync(g_sdk, &handleGotUserSettings, NULL);
}

wchar_t const* GetCurrentPermissionStr()
{
    return g_permissionStr;
}

wchar_t const* GetLastOverlayEvent()
{
    return g_overlayStateStr;
}

wchar_t const* GetLastResult()
{
    return g_lastResult;
}

wchar_t const* GetLastQueryResult()
{
    return g_lastQueryResult;
}

void InitGfeSdkWrapper(GfeSdkWrapper* hl)
{
    hl->Init = &Init;
    hl->DeInit = &DeInit;
    hl->OnTick = &OnTick;
    hl->OnOpenGroup = &OnOpenGroup;
    hl->OnCloseGroup = &OnCloseGroup;
    hl->OnSaveScreenshot = &OnSaveScreenshot;
    hl->OnSaveVideo = &OnSaveVideo;
    hl->OnGetNumHighlights = &OnGetNumHighlights;
    hl->OnOpenSummary = &OnOpenSummary;
    hl->OnRequestLanguage = &OnRequestLanguage;
    hl->OnRequestUserSettings = &OnRequestUserSettings;
    hl->GetCurrentPermissionStr = &GetCurrentPermissionStr;
    hl->GetLastOverlayEvent = &GetLastOverlayEvent;
    hl->GetLastQueryResult = &GetLastQueryResult;
    hl->GetLastResult = &GetLastResult;
}

void __stdcall handleConfigured(NVGSDK_RetCode rc, void* context)
{
    updateResultString(rc);

    if (NVGSDK_FAILED(rc))
    {
        return;
    }

    NVGSDK_HighlightConfigParams* params = context;

    for (size_t i = 0; i < params->highlightTableSize; ++i)
    {
        for (size_t name = 0; name < params->highlightDefinitionTable[i].nameTableSize; ++name)
        {
            free((void*)params->highlightDefinitionTable[i].nameTable[name].localeCode);
            free((void*)params->highlightDefinitionTable[i].nameTable[name].localizedString);
        }
        free(params->highlightDefinitionTable[i].nameTable);
    }

    free(params->highlightDefinitionTable);
    free(params);
}

void ConfigureHighlights(char const* defaultLocale, NVGSDK_Highlight* hl, size_t numHighlights)
{
    // Re-creating the highlights table to show the sample code

    //! [ConfigureHighlights C]
    NVGSDK_Highlight* highlights = calloc(numHighlights, sizeof(NVGSDK_Highlight));
    highlights[0].userInterest = false;

    NVGSDK_HighlightConfigParams* params = calloc(1, sizeof(NVGSDK_HighlightConfigParams));
    params->defaultLocale = defaultLocale;
    params->highlightDefinitionTable = highlights;
    params->highlightTableSize = numHighlights;

    for (size_t i = 0; i < numHighlights; ++i)
    {
        highlights[i].id = hl[i].id;
        highlights[i].highlightTags = hl[i].highlightTags;
        highlights[i].significance = hl[i].significance;
        highlights[i].userInterest = hl[i].userInterest;

        highlights[i].nameTableSize = hl[i].nameTableSize;
        highlights[i].nameTable = hl[i].nameTableSize > 0 ? calloc(hl[i].nameTableSize, sizeof(NVGSDK_LocalizedPair)) : NULL;
        for (size_t name = 0; name < hl[i].nameTableSize; ++name)
        {
            highlights[i].nameTable[name].localeCode = calloc(NVGSDK_MAX_LENGTH, sizeof(char));
            strncpy_s((char*)highlights[i].nameTable[name].localeCode, NVGSDK_MAX_LENGTH, hl[i].nameTable[name].localeCode, NVGSDK_MAX_LENGTH);
            highlights[i].nameTable[name].localizedString = calloc(NVGSDK_MAX_LENGTH, sizeof(char));
            strncpy_s((char*)highlights[i].nameTable[name].localizedString, NVGSDK_MAX_LENGTH, hl[i].nameTable[name].localizedString, NVGSDK_MAX_LENGTH);
        }
    }

    NVGSDK_Highlights_ConfigureAsync(g_sdk, params, &handleConfigured, params);
    //! [ConfigureHighlights C]
}

void __stdcall handleNotification(NVGSDK_NotificationType type, NVGSDK_Notification const* response, void* context)
{
    switch (type)
    {
    case NVGSDK_NOTIFICATION_PERMISSIONS_CHANGED:
        handlePermissionChanged(response->permissionsChanged.scopePermissionTable, response->permissionsChanged.scopePermissionTableSize);
        break;
    case NVGSDK_NOTIFICATION_OVERLAY_STATE_CHANGED:
    {
        size_t endOfStr = 0;
        switch (response->overlayStateChanged.state)
        {
        case NVGSDK_OVERLAY_STATE_MAIN:                 endOfStr += swprintf(g_overlayStateStr, NVGSDK_MAX_LENGTH, L"%hs", "Main Overlay Window"); break;
        case NVGSDK_OVERLAY_STATE_PERMISSION:           endOfStr += swprintf(g_overlayStateStr, NVGSDK_MAX_LENGTH, L"%hs", "Permission Overlay Window"); break;
        case NVGSDK_OVERLAY_STATE_HIGHLIGHTS_SUMMARY:   endOfStr += swprintf(g_overlayStateStr, NVGSDK_MAX_LENGTH, L"%hs", "Highlights Summary Overlay Window"); break;
        default:                                        endOfStr += swprintf(g_overlayStateStr, NVGSDK_MAX_LENGTH, L"%hs", "Unknown Window"); break;
        }

        swprintf(g_overlayStateStr + endOfStr, NVGSDK_MAX_LENGTH - endOfStr, L" %hs", response->overlayStateChanged.open ? "OPEN" : "CLOSED");
        break;
    }
    default:
        LOG("Unknown notification type");
        break;
    }
}

void __stdcall handlePermissionChanged(NVGSDK_ScopePermission* scopePermissionTable, size_t size)
{
    NVGSDK_Permission permission = NVGSDK_PERMISSION_MUST_ASK;
    for (size_t i = 0; i < size; ++i)
    {
        if (scopePermissionTable[i].scope == NVGSDK_SCOPE_HIGHLIGHTS_VIDEO)
        {
            permission = scopePermissionTable[i].permission;
        }
    }

    switch (permission)
    {
    case NVGSDK_PERMISSION_MUST_ASK:    swprintf(g_permissionStr, NVGSDK_MAX_LENGTH, L"%hs", "Must Ask"); break;
    case NVGSDK_PERMISSION_GRANTED:     swprintf(g_permissionStr, NVGSDK_MAX_LENGTH, L"%hs", "Granted"); break;
    case NVGSDK_PERMISSION_DENIED:      swprintf(g_permissionStr, NVGSDK_MAX_LENGTH, L"%hs", "Denied"); break;
    default:                            swprintf(g_permissionStr, NVGSDK_MAX_LENGTH, L"%hs", "Unknown"); break;
    }
}

void __stdcall handleGenericResponse(NVGSDK_RetCode rc, void* context)
{
    updateResultString(rc);
}

void updateResultString(NVGSDK_RetCode rc)
{
    swprintf(g_lastResult, NVGSDK_MAX_LENGTH, L"%hs", NVGSDK_RetCodeToString(rc));
}
