/* Copyright (c) 2016-2017, NVIDIA CORPORATION.  All rights reserved.
 *
 * NVIDIA CORPORATION and its licensors retain all intellectual property
 * and proprietary rights in and to this software, related documentation
 * and any modifications thereto.  Any use, reproduction, disclosure or
 * distribution of this software and related documentation without an express
 * license agreement from NVIDIA CORPORATION is strictly prohibited.
 */

#ifndef GFESDK_OS_DEFINES_H
#define GFESDK_OS_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined _WIN64
#   if defined(NVGSDK_COMPILE_DLL)
#       define NVGSDK_EXPORT __declspec(dllexport)
#   else
#       define NVGSDK_EXPORT __declspec(dllimport)
#   endif

#   define NVGSDKApi __cdecl
#   define NVGSDK_INTERFACE struct __declspec(novtable)
#else
#error Add the appropriate construct for the platform complier
#endif

// Older versions of MSVC don't support inline in C
#if defined(_MSC_VER) && !defined(__cplusplus)
#   define inline __inline
#endif

#ifdef __cplusplus
};
#endif

#endif // GFESDK_OS_DEFINES_H
