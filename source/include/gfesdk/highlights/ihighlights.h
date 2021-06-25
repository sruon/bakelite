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
* C GfeSDK Highlights API
*/

#ifndef GFESDK_IHIGHLIGHTS_H
#define GFESDK_IHIGHLIGHTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <gfesdk/highlights/highlights_types.h>

/*!
* Gives a list of possible Highlights to GFE. The user will be able to see this list and determine which of them
* she cares about. Future calls to \ref NVGSDK_SetVideoHighlight and \ref NVGSDK_SetScreenshotHighlight will refer to an
* entry in this list by name.
*
* \param handle[in] - Valid SDK handle returned by NVGSDK_Create
* \param config[in] - Table of possible Highlights
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_ConfigureAsync(NVGSDK_HANDLE* handle, NVGSDK_HighlightConfigParams const* config, NVGSDK_EmptyCallback callback, void* context);

/*!
* The user is able to enable/disable Highlights. This gives the game that information.
*
* \param handle[in]  - Valid SDK handle returned by NVGSDK_Create
* \param userSettings[out] - On NVGSDK_SUCCESS, will be populated with the user settings.
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_GetUserSettingsAsync(NVGSDK_HANDLE* handle, NVGSDK_Highlights_GetUserSettingsCallback cb, void* cbContext);

/*!
* Begins a "group" which groups several Highlights together. As an example, an online game will commonly consist
* of a round of action, followed by a down period sitting in the lobby. The game can choose to define a Group as
* the time between starting and finishing this round of action. At the end, all of the highlights recorded
* during that group may be displayed to the user in a group summary.
*
* \param handle[in] - Valid SDK handle returned by NVGSDK_Create
* \param params[in] - Parameter struct containing the name of the group and additional metadata
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_OpenGroupAsync(NVGSDK_HANDLE* handle, NVGSDK_HighlightOpenGroupParams const* params, NVGSDK_EmptyCallback callback, void* context);

/*!
* Closes out a group and purges the unsaved contents. See @ref NVGSDK_Highlights_OpenGroupAsync
*
* \param handle[in]    - Valid SDK handle returned by NVGSDK_Create
* \param groupName[in] - Parameter struct containing the name of the group and additional close instructions
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_CloseGroupAsync(NVGSDK_HANDLE* handle, NVGSDK_HighlightCloseGroupParams const* params, NVGSDK_EmptyCallback callback, void* context);

/*!
* Records a screenshot highlight for the given group. Attached metadata to it to make the Highlight more interesting.
*
* \param handle[in]      - Valid SDK handle returned by NVGSDK_Create
* \param params[in]      - GroupID and metadata to attach to the highlight
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_SetScreenshotHighlightAsync(NVGSDK_HANDLE* handle, NVGSDK_ScreenshotHighlightParams const* params, NVGSDK_EmptyCallback callback, void* context);

/*!
* Records a video highlight for the given group. Attached metadata to it to make the Highlight more interesting.
* Set the start and end delta to change the length
* of the video clip.
*
* Examples:
* * The API call is made at T+0, with startDelta at -5000 and endDelta 5000. The start of the desired highlight will be
* at T-5s, and the clip will be 10s long.
* * The API call is made at T+0, with startDelta at -10000 and endDelta -5000. The start of the desired highlight will be
* at T-10s, and the clip will be 5s long.
* * The API call is made at T+0, with startDelta at 5000 and endDelta 10000. The start of the desired highlight will be
* at T+5s, and the clip will be 5s long.
*
* \param handle[in]      - Valid SDK handle returned by NVGSDK_Create
* \param params[in]      - GroupID and metadata to attach to the highlight
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*
* \retval NVGSDK_SUCCESS - Call was successful
* \retval NVGSDK_ERR_INVALID_PARAMETER - Invalid handle or other pointer provided
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_SetVideoHighlightAsync(NVGSDK_HANDLE* handle, NVGSDK_VideoHighlightParams const*, NVGSDK_EmptyCallback callback, void* context);

/*!
* Opens up Summary Dialog for one or more groups
*
* \param handle[in] - Valid SDK handle returned by NVGSDK_Create
* \param params[in] - Struct containing the group names and filter params
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_OpenSummaryAsync(NVGSDK_HANDLE* handle, NVGSDK_SummaryParams const* params, NVGSDK_EmptyCallback callback, void* context);

/*!
* Retrieves the number of highlights given the group ID and filtering params
*
* \param handle[in]      - Valid SDK handle returned by NVGSDK_Create
* \param groupParams[in] - Struct containing the group names and filter params
*
* \param callback[in] - Callback return function as described by \ref section_async
* \param context[in]  - Opaque context pointer as described by \ref section_async
*/
NVGSDK_EXPORT void NVGSDKApi
NVGSDK_Highlights_GetNumberOfHighlightsAsync(NVGSDK_HANDLE* handle, NVGSDK_GroupView const* groupParams, NVGSDK_Highlights_GetNumberOfHighlightsCallback cb, void* context);

#ifdef __cplusplus
}
#endif

#endif // GFESDK_IHIGHLIGHTS_H
