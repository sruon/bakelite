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
 * GFE SDK Types. NOTE: Also see sdk_types_ipc.h for types defined by the GFE backend.
 */

#ifndef GFESDK_SDK_TYPES_H
#define GFESDK_SDK_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <gfesdk/os_defines.h>
#include <gfesdk/sdk_types_ipc.h>

#define NVGSDK_MAX_LENGTH 256
size_t const NVGSDK_MAX_LOCALE = 128;
size_t const NVGSDK_MAX_HIGHLIGHT_TABLE = 128;

struct NVGSDK_HANDLE;
typedef struct NVGSDK_HANDLE NVGSDK_HANDLE;

/*! Version returned by GfeSDK backend. See \ref section_version for more info */
typedef struct
{
    uint16_t major;
    uint16_t minor;
    char const* gfeVersionStr;  //!< Current version installed by the user
} NVGSDK_Version;

/*! Possible log levels */
typedef enum
{
    NVGSDK_LOG_NONE  = 0,
    NVGSDK_LOG_ERROR = 1,
    NVGSDK_LOG_INFO  = 2,
    NVGSDK_LOG_DEBUG = 3,
    NVGSDK_LOG_VERBOSE = 4,
    NVGSDK_LOG_MAX
} NVGSDK_LogLevel;

/*! Types of unsolicited notifications */
typedef enum
{
    NVGSDK_NOTIFICATION_PERMISSIONS_CHANGED,    //!< Sent when the user accepts/denies the permissions dialog, or toggles in GFE3
    NVGSDK_NOTIFICATION_OVERLAY_STATE_CHANGED,  //!< Sent when the in-game overlay is shown or removed
    NVGSDK_NOTIFICATION_MAX
} NVGSDK_NotificationType;

/*! Callback function to recieve log messages */
typedef void (__stdcall* NVGSDK_LoggingCallback)(NVGSDK_LogLevel level, char const* message);

/*! Translation definition */
typedef struct
{
    char const* localeCode;         //!< locale code, such as en-us, fr-fr, etc
    char const* localizedString;    //!< String translated for given locale. Max length @ref NVGSDK_MAX_LENGTH - 1
} NVGSDK_LocalizedPair;

/*! Pairs of scope and the permission level given to them */
typedef struct
{
    NVGSDK_Scope scope;
    NVGSDK_Permission permission;
} NVGSDK_ScopePermission;

//////////// Notifications

/*! Notification sent when the user changes their permissions given to the app */
typedef struct
{
    NVGSDK_ScopePermission* scopePermissionTable;
    size_t scopePermissionTableSize;
} NVGSDK_PermissionsChangedNotification;

/*! Notification sent when the in-game overlay is shown or removed */
typedef struct
{
    bool open;                  //!< true if the overlay is opening, false if it is closing
    NVGSDK_OverlayState state;  //!< State the overlay is closing from or opening to
} NVGSDK_OverlayStateChangedNotification;

/*! Union providing switched data depending on which notification occurred. See \ref section_notifications */
typedef struct
{
    void* context;
    union
    {
        NVGSDK_PermissionsChangedNotification permissionsChanged;
        NVGSDK_OverlayStateChangedNotification overlayStateChanged;
    };
} NVGSDK_Notification;
typedef void(__stdcall* NVGSDK_NotificationCallback)(NVGSDK_NotificationType, NVGSDK_Notification const* response, void* context);

/*! Parameters used to setup an SDK connection, given to \ref NVGSDK_Create */
typedef struct
{
    char const* appName;        //!< Provide the name of the application. Will be used in cases when NVIDIA cannot detect game.
    NVGSDK_Scope* scopeTable;   //!< List of scopes used by the application
    size_t scopeTableSize;      //!< Number of entries in the scope table
    bool pollForCallbacks;      /*!< \brief Set to true to poll for asynchronous callbacks on an app thread.
                                 *   If false, callbacks will occur on a GfeSDK controlled thread, no polling necessary */
    NVGSDK_NotificationCallback notificationCallback;   //!< Called on unsolicited notifications
    void* notificationCallbackContext;                  //!< Passed along unchanged with each notification

    uint32_t targetPid;         /*!< \brief Used for linking to game-related process's PID that also supports NVIDIA Highlights. 
                                *   Leave empty when not used. */
    char const* targetPath;     /*!< \brief Used for linking to game-related process's path that also supports NVIDIA Highlights. 
                                * Must be defined if targetPid is defined. Leave empty when not used. */
} NVGSDK_CreateInputParams;

/*! Parameters given to \ref NVGSDK_RequestPermissionsAsync */
typedef struct
{
    NVGSDK_Scope* scopeTable;
    size_t scopeTableSize;
} NVGSDK_RequestPermissionsParams;

/////////////// Responses from API calls

/*! Response from \ref NVGSDK_Create call */
typedef struct
{
    uint16_t versionMajor;                          //!< Will be populated with SDK major version
    uint16_t versionMinor;                          //!< Will be populated with SDK minor version
    char gfeVersionStr[NVGSDK_MAX_LENGTH];          //!< Must be provided to call. Will be populated by GFE version string
    NVGSDK_ScopePermission* scopePermissionTable;   //!< Must be provided to call. Will be filled with scope/permission pairs
    size_t scopePermissionTableSize;                //!< Must be provided to call.
} NVGSDK_CreateResponse;
/*! Type of callback to provide to most asynchronous APIs that don't return extra data */
typedef void(__stdcall* NVGSDK_EmptyCallback)(NVGSDK_RetCode, void*);

/*! Gets populated with the user's GFE language choice */
typedef struct
{
    char const* cultureCode;  //!< Will be populated by the user's GFE language selection
} NVGSDK_Language;
/*! Type of callback to provide to NVGSDK_GetUILanguage */
typedef void(__stdcall* NVGSDK_GetUILanguageCallback)(NVGSDK_RetCode, NVGSDK_Language const*, void*);

#endif // GFESDK_SDK_TYPES_H
