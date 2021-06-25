/* Copyright (c) 2017, NVIDIA CORPORATION.  All rights reserved.
*
* NVIDIA CORPORATION and its licensors retain all intellectual property
* and proprietary rights in and to this software, related documentation
* and any modifications thereto.  Any use, reproduction, disclosure or
* distribution of this software and related documentation without an express
* license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#ifndef GFESDK_SDK_TYPES_CPP_H
#define GFESDK_SDK_TYPES_CPP_H

// Include these first, so they don't get stuck in the GfeSDK namespace below
#include <wchar.h>
#include <stdint.h>
#include <stdbool.h>

// Avoid polluting global namespace with C api
namespace GfeSDK
{
#include <gfesdk/sdk_types.h>
}

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace GfeSDK
{

struct NotificationBase;
typedef std::vector<NVGSDK_Scope> TScopes;
typedef std::map<NVGSDK_Scope, NVGSDK_Permission> TScopePermissions;
typedef std::map<std::string, std::string> TTranslationTable;
typedef std::function<void(NVGSDK_NotificationType, NotificationBase const&)> TNotificationCallback;

struct CreateInputParams
{
    std::string appName;
    TScopes requiredScopes;
    bool pollForCallbacks;
    TNotificationCallback notificationCallback;
    void* notificationCallbackContext;

    uint32_t targetPid;         //!< Advanced usage
    std::string targetPath;     //!< Advanced usage

    CreateInputParams() : pollForCallbacks(false), notificationCallback(nullptr), notificationCallbackContext(nullptr), targetPid(0) {}
};

struct RequestPermissionsParams
{
    TScopes scopes;
};

/////////////// Responses from API calls
typedef std::function<void(NVGSDK_RetCode, void*)> TCallback;

//! C++ binding for @ref NVGSDK_CreateResponse
struct CreateResponse
{
    NVGSDK_RetCode returnCode;
    uint16_t versionMajor;
    uint16_t versionMinor;
    std::string nvidiaGfeVersion;
    TScopePermissions scopePermissions;
};

//! C++ binding for @ref NVGSDK_GetUILanguageResponse
struct GetUILanguageResponse
{
    std::string cultureCode;
};
typedef std::function<void(NVGSDK_RetCode rc, GetUILanguageResponse const*, void*)> TGetUILanguageCallback;

/////////////// Notifications
struct NotificationBase
{
    void* context;
};

struct PermissionsChangedNotification : public NotificationBase
{
    TScopePermissions scopePermissions;
};

struct OverlayStateChangedNotification : public NotificationBase
{
    bool open;
    NVGSDK_OverlayState state;
};

}

#endif //GFESDK_SDK_TYPES_CPP_H
