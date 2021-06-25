// Copyright (c) 2016-2019, NVIDIA CORPORATION.  All rights reserved.
//
// NVIDIA CORPORATION and its licensors retain all intellectual property
// and proprietary rights in and to this software, related documentation
// and any modifications thereto.  Any use, reproduction, disclosure or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA CORPORATION is strictly prohibited.

#ifndef NVSDKAPI_IPC_H
#define NVSDKAPI_IPC_H

#include <wchar.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * The API distributed with the game represents a single version of the GfeSDK.
     * The installed version of GFE may or may not be compatible with this version. A
     * version check is initiated when the SDK is initialized.
     *
     * Whenever the API between the SDK and GFE backend gets updated, one of these version numbers
     * gets bumped as follows:
     *  MAJOR.MINOR
     * MINOR - An API has changed, but the old API is still being supported for backward compatibility
     *  Example: An existing call is reconfigured, or an optional field is added
     *    Case 1: SDK DLL version and GFE Backend version are equal. No problem
     *      Creation will return NVGSDK_SUCCESS
     *    Case 2: SDK DLL version is 0.2, and GFE Backend version is 0.1
     *      API is likely to function as expected, but functionality new to 0.2 won't be expected
     *      to work. Game should consider asking the user to upgrade GFE
     *      Creation will return NVGSDK_SUCCESS_VERSION_OLD_GFE
     *    Case 3: SDK DLL version is 0.1 and GFE Backend version is 0.2. The game needs to update
     *      the distributed DLL to get access to the latest feature, but otherwise it will work correctly.
     *      The game should update to latest SDK version when possible.
     *      Creation will return NVGSDK_SUCCESS_VERSION_OLD_SDK
     * MAJOR - The update will break backward compatibility completely. THIS IS NEVER TO BE USED IF POSSIBLE
     *    Case 1: SDK DLL version and GFE Backend version are equal. No problem
     *      Creation will return NVGSDK_SUCCESS
     *    Case 2: SDK DLL version is 1.0, and GFE Backend version is 0.1.
     *      SDK won't work as expected. GFE3 must be upgraded.
     *      Creation will return NVGSDK_ERR_GFE_VERSION
     *    Case 3: SDK DLL version is 0.1 and GFE Backend version is 1.0.
     *      SDK won't work as expected. Game must ship with a newer SDK DLL.
     *      Creation will return NVGSDK_ERR_SDK_VERSION
     */

    int const NVGSDK_IPC_VERSION_MAJOR = 1;
    int const NVGSDK_IPC_VERSION_MINOR = 1;

    /*!
     * \brief Possible return values of all external APIs
     */
    typedef enum _NVGSDK_RetCode
    {
        NVGSDK_SUCCESS = 0,
        NVGSDK_SUCCESS_VERSION_OLD_SDK = 1001,
        NVGSDK_SUCCESS_VERSION_OLD_GFE = 1002,
        NVGSDK_SUCCESS_PENDING = 1003,
        NVGSDK_SUCCESS_USER_NOT_INTERESTED = 1004,
        NVGSDK_SUCCESS_PERMISSION_GRANTED = 1005,
        NVGSDK_SUCCESS_LINKED = 1006,

        NVGSDK_ERR_GENERIC = -1001,
        NVGSDK_ERR_GFE_VERSION = -1002,
        NVGSDK_ERR_SDK_VERSION = -1003,
        NVGSDK_ERR_NOT_IMPLEMENTED = -1004,
        NVGSDK_ERR_INVALID_PARAMETER = -1005,
        NVGSDK_ERR_NOT_SET = -1006,
        NVGSDK_ERR_SHADOWPLAY_IR_DISABLED = -1007,
        NVGSDK_ERR_SDK_IN_USE = -1008,
        NVGSDK_ERR_GROUP_NOT_FOUND = -1009,
        NVGSDK_ERR_FILE_NOT_FOUND = -1010,
        NVGSDK_ERR_HIGHLIGHTS_SETUP_FAILED = -1011,
        NVGSDK_ERR_HIGHLIGHTS_NOT_CONFIGURED = -1012,
        NVGSDK_ERR_HIGHLIGHTS_SAVE_FAILED = -1013,
        NVGSDK_ERR_UNEXPECTED_EXCEPTION = -1014,
        NVGSDK_ERR_NO_HIGHLIGHTS = -1015,
        NVGSDK_ERR_NO_CONNECTION = -1016,
        NVGSDK_ERR_PERMISSION_NOT_GRANTED = -1017,
        NVGSDK_ERR_PERMISSION_DENIED = -1018,
        NVGSDK_ERR_INVALID_HANDLE = -1019,
        NVGSDK_ERR_UNHANDLED_EXCEPTION = -1020,
        NVGSDK_ERR_OUT_OF_MEMORY = -1021,
        NVGSDK_ERR_LOAD_LIBRARY = -1022,
        NVGSDK_ERR_LIB_CALL_FAILED = -1023,
        NVGSDK_ERR_IPC_FAILED = -1024,
        NVGSDK_ERR_CONNECTION = -1025,
        NVGSDK_ERR_MODULE_NOT_LOADED = -1026,
        NVGSDK_ERR_LIB_CALL_TIMEOUT = -1027,
        NVGSDK_ERR_APPLICATION_LOOKUP_FAILED = -1028,
        NVGSDK_ERR_APPLICATION_NOT_KNOWN = -1029,
        NVGSDK_ERR_FEATURE_DISABLED = -1030,
        NVGSDK_ERR_APP_NO_OPTIMIZATION = -1031,
        NVGSDK_ERR_APP_SETTINGS_READ = -1032,
        NVGSDK_ERR_APP_SETTINGS_WRITE = -1033,
        NVGSDK_ERR_HIGHLIGHT_NOT_DEFINED = -1034,
        NVGSDK_ERR_CAP_LIMIT_REACHED = -1035,
        NVGSDK_ERR_DISK_LIMIT_REACHED = -1036
    } NVGSDK_RetCode;

    // not using macros to avoid polluting namespace of SDK unnecessarily

    /*! NVGSDK_RetCode success function */
    inline bool NVGSDK_SUCCEEDED(NVGSDK_RetCode r)
    {
        return r >= 0;
    }

    /*! NVGSDK_RetCode failure function */
    inline bool NVGSDK_FAILED(NVGSDK_RetCode r)
    {
        return r < 0;
    }

    /*! Possible values for highlight types */
    typedef enum
    {
        /*!
         * No tags are attached to the highlight
         */
        NVGSDK_HIGHLIGHT_TYPE_NONE = 0x00000000,
        /*!
         * Signifies direct progress towards completing the game. (e.g. completing a level)
         */
        NVGSDK_HIGHLIGHT_TYPE_MILESTONE = 0x00000001,
        /*!
         * Player accomplishment of extra challenges independent of progress in completing a game.
         * (e.g. perfect score in a level, found the easter egg)
         */
        NVGSDK_HIGHLIGHT_TYPE_ACHIEVEMENT = 0x00000002,
        /*!
         * Event not significant in competion of game. (e.g. player kills a minion)
         */
        NVGSDK_HIGHLIGHT_TYPE_INCIDENT = 0x00000004,
        /*!
         * Change in player state, triggered by player or externally by game.
         * (e.g. equipping a weapon)
         */
        NVGSDK_HIGHLIGHT_TYPE_STATE_CHANGE = 0x00000008,
        /*!
         * Special highlight type that does not notify the user as it happens, to avoid giving
         * the user a competetive advantage. e.g. A game doesn't tell the user that they recorded
         * a kill, but wants the highlight later for the user after the round is over.
         */
        NVGSDK_HIGHLIGHT_TYPE_UNANNOUNCED = 0x00000010,
        //! Invalid default value
        NVGSDK_HIGHLIGHT_TYPE_MAX = 0x00000020
    } NVGSDK_HighlightType;

    /*! Possible significance values */
    typedef enum
    {
        /*! No significance is attached to the highlights */
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_NONE = 0x00000000,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_EXTREMELY_BAD = 0x00000001,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_VERY_BAD = 0x00000002,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_BAD = 0x00000004,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_NEUTRAL = 0x00000010,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_GOOD = 0x00000100,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_VERY_GOOD = 0x00000200,
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_EXTREMELY_GOOD = 0x00000400,

        //! Invalid default value
        NVGSDK_HIGHLIGHT_SIGNIFICANCE_MAX = 0x00000800
    } NVGSDK_HighlightSignificance;

    /*! Functionality supported by the SDK and possibly requiring user permission */
    typedef enum
    {
        NVGSDK_SCOPE_HIGHLIGHTS = 0,       //!< Scope required to access highlights information, open group summary, etc
        NVGSDK_SCOPE_HIGHLIGHTS_VIDEO = 1, //!< Scope required to save video highlights
        NVGSDK_SCOPE_HIGHLIGHTS_SCREENSHOT = 2, //!< Scope required to save screenshot highlights
        NVGSDK_SCOPE_OPS = 3,                   //!< Scope required to optimize the game
        NVGSDK_SCOPE_MAX
    } NVGSDK_Scope;

    /*! Possible permissions that may be implicitly given by GFE, or explicitly decided by the user */
    typedef enum
    {
        NVGSDK_PERMISSION_GRANTED = 0,  //!< Requested permission has been granted to the app by the user
        NVGSDK_PERMISSION_MUST_ASK = 1, //!< Requested permission must be granted by the user.
        NVGSDK_PERMISSION_DENIED = 2,   //!< Requested permission has been denied to the app, feature won't function
        NVGSDK_PERMISSION_UNKNOWN = 3,  //!< Scope does not match list of known scopes. Version mismatch?
        NVGSDK_PERMISSION_MAX
    } NVGSDK_Permission;

    /*! State that the overlay can be in when it opens and closes */
    typedef enum
    {
        NVGSDK_OVERLAY_STATE_MAIN,               //!< Normal overlay menu
        NVGSDK_OVERLAY_STATE_PERMISSION,         //!< Menu that appears to ask user for permissions
        NVGSDK_OVERLAY_STATE_HIGHLIGHTS_SUMMARY, //!< Menu that appears to show the user the highlights group summary
        NVGSDK_OVERLAY_STATE_MAX
    } NVGSDK_OverlayState;

    /*!
     * Converts the code returns from an NVGSDK_* API to a printable string
     */
    inline const char *NVGSDK_RetCodeToString(NVGSDK_RetCode const ret)
    {
        switch (ret)
        {
        case NVGSDK_SUCCESS:
            return "NVGSDK_SUCCESS";
        case NVGSDK_SUCCESS_VERSION_OLD_SDK:
            return "NVGSDK_SUCCESS_VERSION_OLD_SDK";
        case NVGSDK_SUCCESS_VERSION_OLD_GFE:
            return "NVGSDK_SUCCESS_VERSION_OLD_GFE";
        case NVGSDK_SUCCESS_PENDING:
            return "NVGSDK_SUCCESS_PENDING";
        case NVGSDK_SUCCESS_USER_NOT_INTERESTED:
            return "NVGSDK_SUCCESS_USER_NOT_INTERESTED";
        case NVGSDK_SUCCESS_PERMISSION_GRANTED:
            return "NVGSDK_SUCCESS_PERMISSION_GRANTED";
        case NVGSDK_SUCCESS_LINKED:
            return "NVGSDK_SUCCESS_LINKED";
        case NVGSDK_ERR_GENERIC:
            return "NVGSDK_ERR_GENERIC";
        case NVGSDK_ERR_GFE_VERSION:
            return "NVGSDK_ERR_GFE_VERSION";
        case NVGSDK_ERR_SDK_VERSION:
            return "NVGSDK_ERR_SDK_VERSION";
        case NVGSDK_ERR_NOT_IMPLEMENTED:
            return "NVGSDK_ERR_NOT_IMPLEMENTED";
        case NVGSDK_ERR_INVALID_PARAMETER:
            return "NVGSDK_ERR_INVALID_PARAMETER";
        case NVGSDK_ERR_NOT_SET:
            return "NVGSDK_ERR_NOT_SET";
        case NVGSDK_ERR_SHADOWPLAY_IR_DISABLED:
            return "NVGSDK_ERR_SHADOWPLAY_IR_DISABLED";
        case NVGSDK_ERR_SDK_IN_USE:
            return "NVGSDK_ERR_SDK_IN_USE";
        case NVGSDK_ERR_GROUP_NOT_FOUND:
            return "NVGSDK_ERR_GROUP_NOT_FOUND";
        case NVGSDK_ERR_FILE_NOT_FOUND:
            return "NVGSDK_ERR_FILE_NOT_FOUND";
        case NVGSDK_ERR_HIGHLIGHTS_SETUP_FAILED:
            return "NVGSDK_ERR_HIGHLIGHTS_SETUP_FAILED";
        case NVGSDK_ERR_HIGHLIGHTS_NOT_CONFIGURED:
            return "NVGSDK_ERR_HIGHLIGHTS_NOT_CONFIGURED";
        case NVGSDK_ERR_HIGHLIGHTS_SAVE_FAILED:
            return "NVGSDK_ERR_HIGHLIGHTS_SAVE_FAILED";
        case NVGSDK_ERR_UNEXPECTED_EXCEPTION:
            return "NVGSDK_ERR_UNEXPECTED_EXCEPTION";
        case NVGSDK_ERR_NO_HIGHLIGHTS:
            return "NVGSDK_ERR_NO_HIGHLIGHTS";
        case NVGSDK_ERR_NO_CONNECTION:
            return "NVGSDK_ERR_NO_CONNECTION";
        case NVGSDK_ERR_PERMISSION_NOT_GRANTED:
            return "NVGSDK_ERR_PERMISSION_NOT_GRANTED";
        case NVGSDK_ERR_PERMISSION_DENIED:
            return "NVGSDK_ERR_PERMISSION_DENIED";
        case NVGSDK_ERR_INVALID_HANDLE:
            return "NVGSDK_ERR_INVALID_HANDLE";
        case NVGSDK_ERR_UNHANDLED_EXCEPTION:
            return "NVGSDK_ERR_UNHANDLED_EXCEPTION";
        case NVGSDK_ERR_OUT_OF_MEMORY:
            return "NVGSDK_ERR_OUT_OF_MEMORY";
        case NVGSDK_ERR_LOAD_LIBRARY:
            return "NVGSDK_ERR_LOAD_LIBRARY";
        case NVGSDK_ERR_LIB_CALL_FAILED:
            return "NVGSDK_ERR_LIB_CALL_FAILED";
        case NVGSDK_ERR_IPC_FAILED:
            return "NVGSDK_ERR_IPC_FAILED";
        case NVGSDK_ERR_CONNECTION:
            return "NVGSDK_ERR_CONNECTION";
        case NVGSDK_ERR_MODULE_NOT_LOADED:
            return "NVGSDK_ERR_MODULE_NOT_LOADED";
        case NVGSDK_ERR_LIB_CALL_TIMEOUT:
            return "NVGSDK_ERR_LIB_CALL_TIMEOUT";
        case NVGSDK_ERR_APPLICATION_LOOKUP_FAILED:
            return "NVGSDK_ERR_APPLICATION_LOOKUP_FAILED";
        case NVGSDK_ERR_APPLICATION_NOT_KNOWN:
            return "NVGSDK_ERR_APPLICATION_NOT_KNOWN";
        case NVGSDK_ERR_FEATURE_DISABLED:
            return "NVGSDK_ERR_FEATURE_DISABLED";
        case NVGSDK_ERR_APP_NO_OPTIMIZATION:
            return "NVGSDK_ERR_APP_NO_OPTIMIZATION";
        case NVGSDK_ERR_APP_SETTINGS_READ:
            return "NVGSDK_ERR_APP_SETTINGS_READ";
        case NVGSDK_ERR_APP_SETTINGS_WRITE:
            return "NVGSDK_ERR_APP_SETTINGS_WRITE";
        case NVGSDK_ERR_HIGHLIGHT_NOT_DEFINED:
            return "NVGSDK_ERR_HIGHLIGHT_NOT_DEFINED";
        case NVGSDK_ERR_CAP_LIMIT_REACHED:
            return "NVGSDK_ERR_CAP_LIMIT_REACHED";
        case NVGSDK_ERR_DISK_LIMIT_REACHED:
            return "NVGSDK_ERR_DISK_LIMIT_REACHED";
        }
        return "Unknown code";
    }

    // All alphanumeric and the hyphen
    char const VALID_GROUPID_CHARS[] = "^[a-zA-Z0-9\\-\\._ ']+$";
    wchar_t const VALID_GROUPID_WCHARS[] = L"^[a-zA-Z0-9\\-\\._ ']+$";

    char const VALID_HIGHLIGHT_DEF_ID_CHARS[] = "^[a-zA-Z0-9\\-\\._ ']+$";
    wchar_t const VALID_HIGHLIGHT_DEF_ID_WCHARS[] = L"^[a-zA-Z0-9\\-\\._ ']+$";

    char const VALID_GAMENAME_CHARS[] = "^[a-zA-Z0-9\\-\\._ ']+$";
    wchar_t const VALID_GAMENAME_WCHARS[] = L"^[a-zA-Z0-9\\-\\._ ']+$";

    char const VALID_LOCALE_CODE_CHARS[] = "^[a-zA-Z\\-]+$";
    wchar_t const VALID_LOCALE_CODE_WCHARS[] = L"^[a-zA-Z\\-]+$";

    size_t const VALID_HIGHLIGHT_SIGNIFICANCE_WIDTH = 0x8FF;
    size_t const VALID_HIGHLIGHT_TAG_WIDTH = 0x1F;

#ifdef __cplusplus
}
#endif

#endif // NVSDKAPI_IPC_H
