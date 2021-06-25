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
 * C++ bindings for GfeSDK.
 */

#ifndef GFESDK_ISDK_CPP_H
#define GFESDK_ISDK_CPP_H

#include "sdk_types_cpp.h"

namespace GfeSDK
{
// Avoid polluting global namespace with C api, so put it inside namespace
#include <gfesdk/isdk.h>

// Static functions that don't depend on an instance of the SDK
// Giving an alias to these to remove the NVGSDK_ prefix in lieu of the namespace
/*! C++ binding for \ref NVGSDK_SetLogLevel */
inline NVGSDK_RetCode SetFileLogLevel(NVGSDK_LogLevel level) { return NVGSDK_SetLogLevel(level); }
/*! C++ binding for \ref NVGSDK_AttachLogListener */
inline NVGSDK_RetCode AttachLogListener(NVGSDK_LoggingCallback callback) { return NVGSDK_AttachLogListener(callback); }
/*! C++ binding for \ref NVGSDK_SetListenerLogLevel */
inline NVGSDK_RetCode SetListenerLogLevel(NVGSDK_LogLevel level) { return NVGSDK_SetListenerLogLevel(level); }
/*! C++ binding for NVGSDK_RetCodeToString */
inline const char* RetCodeToString(NVGSDK_RetCode rc) { return NVGSDK_RetCodeToString(rc); }

/*! C++ Core interface */
class Core
{
public:
    /*! C++ binding for \ref NVGSDK_Create */
    static Core* Create(CreateInputParams const&, CreateResponse&);
    virtual ~Core() {}

    /*! C++ binding for \ref NVGSDK_Poll */
    virtual NVGSDK_RetCode Poll(void) = 0;
    /*! C++ binding for \ref NVGSDK_RequestPermissionsAsync */
    virtual void RequestPermissionsAsync(RequestPermissionsParams const&, TCallback cb = nullptr, void* cbContext = nullptr) = 0;
    /*! C++ binding for \ref NVGSDK_GetUILanguageAsync */
    virtual void GetUILanguageAsync(TGetUILanguageCallback cb = nullptr, void* cbContext = nullptr) = 0;

    // Return the C API handle;
    virtual NVGSDK_HANDLE* GetHandle() = 0;
};
}

#endif //GFESDK_ISDK_CPP_H
