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
 * C GFE SDK API
 */

#ifndef GFESDK_ISDK_H
#define GFESDK_ISDK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gfesdk/os_defines.h>
#include <gfesdk/sdk_types.h>
#include <gfesdk/sdk_types_ipc.h>
#include <gfesdk/config.h>
#include "stdint.h"

/////////////////// CORE

/*!
 * Allocates and initializes the SDK interface. This is a synchronous call and
 * may take tens of milliseconds.
 *
 * Importantly, the app must pass a list of required scopes to this call. This
 * is analagous to an oauth2 type of flow. GfeSDK will choose to allocate
 * certain resources based on this list. For example, passing
 * \ref NVGSDK_SCOPE_HIGHLIGHTS to this list will enable all of the calls
 * starting with NVGSDK_Highlights_*. For more information on permissions, see
 * \ref section_permissions.
 *
 * The next decision for the app is whether to accept asynchronous callbacks on
 * a GfeSDK-controller thread, or to poll for callbacks on a game-controlled
 * thread, usually the main game loop. See
 * \ref NVGSDK_CreateInputParams::pollForCallbacks and
 * \ref section_threading "Threading". When bringing the SDK up for the first
 * time, it may be best to start with pollForCallbacks = false, until up and
 * running, and then move to the more advanced polling model that gives control
 * to the proper thread.
 *
 * Finally, the app can pass in a callback to recieve notifications, and an
 * opaque pointer that will be passed untouched to be received by the callback.
 * See \ref section_notifications
 *
 * \param handle[out]    - Will be populated by the created SDK on success
 * \param inParams[in]   - Input parameters for SDK creation
 * \param outParams[out] - Output params for SDK creation, will be populated on return. On fatal error, this will be NULL
 *
 * \retval NVGSDK_SUCCESS - SDK was successfully created and returned
 * \retval NVGSDK_ERR_OUT_OF_MEMORY     - Could not allocate system resources for handle
 * \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle pointer provided, or no app name provided
 * \retval NVGSDK_ERR_LOAD_LIBRARY      - SDK wasn't able to load dependencies. Perhaps GFE is not installed.
 * \retval NVGSDK_ERR_CONNECTION        - SDK is not able to contact GFE backend service
 * \retval NVGSDK_ERR_UNHANDLED_EXCEPTION - SDK ran into an unhandled error and caught an exception before it
 *                                          returned to client code
 * \retval NVGSDK_IPC_SUCCESS_OLD_SDK     - SDK will work properly, but the GfeSDK DLL version is old.
                                            See \ref section_version
 * \retval NVGSDK_IPC_SUCCESS_OLD_GFE     - SDK should work properly, but the backend GFE version is older,
                                            and may not provide the latest features. See \ref section_version
 * \retval NVGSDK_ERR_GFE_VERSION         - GFE version is too old for communication. User must upgrade GFE.
                                            See \ref section_version
 * \retval NVGSDK_ERR_SDK_VERSION         - GfeSDK DLL version is too old. Game must distribute a newer version.
                                            See \ref section_version
 */
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_Create(NVGSDK_HANDLE** handle, NVGSDK_CreateInputParams const* inParams, NVGSDK_CreateResponse* outParams);

/*!
 * Release the main SDK interface after create.
 *
 * \param pHandle[in] - SDK Handle to release
 *
 * \retval NVGSDK_SUCCESS - SDK was successfully released
 * \retval NVGSDK_ERR_INVALID_PARAMETER   - Invalid handle provided
 * \retval NVGSDK_ERR_UNHANDLED_EXCEPTION - SDK ran into an unhandled error and caught an exception before it
 *                                          returned to client code
 */
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_Release(NVGSDK_HANDLE* handle);

/*!
 * When \ref NVGSDK_CreateInputParams::pollForCallbacks is set to true, it becomes
 * the app's responsibility to make calls to this function to recieve asynchronous
 * callbacks. See \ref section_async for more information.
 *
 * \param handle[in]    - Valid SDK handle returned by NVGSDK_Create
 */
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_Poll(NVGSDK_HANDLE* handle);

/*!
* Adjusts the logging level of GfeSDK. Log level documentation is found at
* \ref NVGSDK_LogLevel. See \ref section_log for more information.
*
* \param fileLvl[in] - Logs at or above this priority will be written to logfile
*/
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_SetLogLevel(NVGSDK_LogLevel fileLevel);

/*!
 * Pass in a callback function in order to receive the log messages that GfeSDK is
 * recording. The primary usecase for this is to get the GfeSDK logs into the logfile
 * of a game engine. See \ref section_log for more information.
 *
 * \param[in] callback - Will be called back with log priority and message
 */
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_AttachLogListener(NVGSDK_LoggingCallback callback);

/*!
* Adjusts the listener logging level of GfeSDK. This affects calls to the callback
* given to \ref NVGSDK_AttachLogListener. See \ref section_log for more information.
*
* See \ref NVGSDK_LogLevel for a list of possible levels.
*
* \param level[in] - Logs at or above this priority will be sent to callback
*/
NVGSDK_EXPORT NVGSDK_RetCode NVGSDKApi NVGSDK_SetListenerLogLevel(NVGSDK_LogLevel level);

/*!
 * Asks the user for permissions needed by the app. This will open up the
 * \ref section_igo.
 *
 * If the user has already decided to grant or deny permissions for these
 * scopes, no overlay will be displayed, and the corresponding return value
 * will be passed into the callback.
 *
 * \param callback[in] - Callback return function as described by \ref section_async
 * \param context[in]  - Opaque context pointer as described by \ref section_async
 */
NVGSDK_EXPORT void NVGSDKApi NVGSDK_RequestPermissionsAsync(NVGSDK_HANDLE* handle, NVGSDK_RequestPermissionsParams const* params, NVGSDK_EmptyCallback callback, void* context);

/*!
 * Returns the language set by the user for the onscreen UI
 *
 * \param handle[in]    - Valid SDK handle returned by NVGSDK_Create
 * \param language[out] - Will be populated on NVGSDK_SUCCESS
 *
 * \param callback[in] - Callback return function as described by \ref section_async
 * \param context[in]  - Opaque context pointer as described by \ref section_async
 *
 * \retval NVGSDK_SUCCESS - Call was successful, and lang was populated with the result
 * \retval NVGSDK_ERR_INVALID_PARAMETER   - Invalid handle provided
 * \retval NVGSDK_ERR_UNHANDLED_EXCEPTION - SDK ran into an unhandled error and caught an exception before it
 *                                          returned to client code
 * \retval NVGSDK_IPC_ERR_NOT_SET - The user has not chosen a language and is using the default
 */
NVGSDK_EXPORT void NVGSDKApi NVGSDK_GetUILanguageAsync(NVGSDK_HANDLE* handle, NVGSDK_GetUILanguageCallback callback, void* context);

#ifdef __cplusplus
}
#endif

#endif // GFESDK_ISDK_H
