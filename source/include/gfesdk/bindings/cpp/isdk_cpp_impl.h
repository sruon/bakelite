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
* Implemented in the header to avoid ABI incompatibilities
*/

#ifndef GFESDK_ISDK_CPP_IMPL_H
#define GFESDK_ISDK_CPP_IMPL_H

#include "isdk_cpp.h"

#include <memory>

namespace GfeSDK
{
class CoreImpl : public Core
{
public:
    ~CoreImpl();

    virtual NVGSDK_RetCode Poll(void);
    virtual void RequestPermissionsAsync(RequestPermissionsParams const&, TCallback cb = nullptr, void* cbContext = nullptr);
    virtual void GetUILanguageAsync(TGetUILanguageCallback cb = nullptr, void* cbContext = nullptr);

    NVGSDK_HANDLE* GetHandle();

    friend class Core;
    friend class HighlightsImpl;
    friend class OpsImpl;
private:
    // Remove default construct, copy and assignment operators
    CoreImpl() {}
    CoreImpl(const CoreImpl& other) {}
    CoreImpl& operator=(const CoreImpl& other) {}

private:
    template <typename T>
    struct CallbackContext
    {
        T callback;
        void* appContext;
    };
    struct CallbackNotificationContext
    {
        TNotificationCallback callback;
        void* appContext;
    };
    static void __stdcall callbackWrapper(NVGSDK_RetCode rc, void* appContext);
    static void __stdcall callbackNotificationWrapper(NVGSDK_NotificationType type, NVGSDK_Notification const* data, void* context);
    static inline void translateLocaleTable(std::map<std::string, std::string> const& localeMap, std::vector<NVGSDK_LocalizedPair>& pairs);

    NVGSDK_HANDLE* m_handle;
    std::unique_ptr<CallbackNotificationContext> m_notificationCallbackContext;
};

inline Core* Core::Create(CreateInputParams const& inParams, CreateResponse& outParams)
{
    CoreImpl* core = new CoreImpl();

    NVGSDK_CreateInputParams cInputParams;
    memset(&cInputParams, 0, sizeof(cInputParams));
    cInputParams.appName = inParams.appName.c_str();
    cInputParams.pollForCallbacks = inParams.pollForCallbacks;
    if (!inParams.requiredScopes.empty())
    {
        cInputParams.scopeTable = const_cast<NVGSDK_Scope*>(&inParams.requiredScopes[0]);
        cInputParams.scopeTableSize = inParams.requiredScopes.size();
    }
    if (inParams.notificationCallback)
    {
        cInputParams.notificationCallback = &CoreImpl::callbackNotificationWrapper;
        core->m_notificationCallbackContext.reset(new CoreImpl::CallbackNotificationContext({ inParams.notificationCallback, inParams.notificationCallbackContext }));
        cInputParams.notificationCallbackContext = core->m_notificationCallbackContext.get();
    }
    cInputParams.targetPid = inParams.targetPid;
    if (!inParams.targetPath.empty())
    {
        cInputParams.targetPath = inParams.targetPath.c_str();
    }

    NVGSDK_CreateResponse cOutputParams = { 0 };
    std::vector<NVGSDK_ScopePermission> cScopePermissionList;
    cScopePermissionList.resize(inParams.requiredScopes.size());
    
    if (!inParams.requiredScopes.empty())
    {
        cOutputParams.scopePermissionTable = &cScopePermissionList[0];
        cOutputParams.scopePermissionTableSize = cScopePermissionList.size();
    }

    outParams.returnCode = NVGSDK_Create(&core->m_handle, &cInputParams, &cOutputParams);
    outParams.versionMajor = cOutputParams.versionMajor;
    outParams.versionMinor = cOutputParams.versionMinor;
    outParams.nvidiaGfeVersion = cOutputParams.gfeVersionStr;

    if (NVGSDK_FAILED(outParams.returnCode))
    {
        delete core;
        return nullptr;
    }

    if (cOutputParams.scopePermissionTable)
    {
        for (size_t i = 0; i < cOutputParams.scopePermissionTableSize; ++i)
        {
            NVGSDK_ScopePermission const& scopePermission = cOutputParams.scopePermissionTable[i];
            outParams.scopePermissions[scopePermission.scope] = scopePermission.permission;
        }
    }

    return core;
}

inline CoreImpl::~CoreImpl()
{
    if (!m_handle)
    {
        return;
    }

    NVGSDK_Release(m_handle);
}

inline NVGSDK_RetCode CoreImpl::Poll(void)
{
    return NVGSDK_Poll(m_handle);
}

inline void CoreImpl::RequestPermissionsAsync(RequestPermissionsParams const& params, TCallback callback, void* context)
{
    NVGSDK_RequestPermissionsParams cParams;
    cParams.scopeTable = const_cast<NVGSDK_Scope*>(&params.scopes[0]);
    cParams.scopeTableSize = params.scopes.size();
    return NVGSDK_RequestPermissionsAsync(m_handle, &cParams, callbackWrapper, new CallbackContext<TCallback>({ callback, context }));
}

inline void CoreImpl::GetUILanguageAsync(TGetUILanguageCallback callback, void* context)
{
    NVGSDK_GetUILanguageAsync(m_handle, [](NVGSDK_RetCode rc, NVGSDK_Language const* data, void* context) {
        std::unique_ptr<CallbackContext<TGetUILanguageCallback>> callbackContext(reinterpret_cast<CallbackContext<TGetUILanguageCallback>*>(context));
        if (!callbackContext->callback) return;

        if (NVGSDK_FAILED(rc))
        {
            return callbackContext->callback(rc, nullptr, callbackContext->appContext);
        }

        GetUILanguageResponse language;
        language.cultureCode = data->cultureCode;
        callbackContext->callback(rc, &language, callbackContext->appContext);
    }, new CallbackContext<TGetUILanguageCallback>({ callback, context }));
}

inline NVGSDK_HANDLE* CoreImpl::GetHandle()
{
    return m_handle;
}

inline void CoreImpl::callbackWrapper(NVGSDK_RetCode rc, void* context)
{
    std::unique_ptr<CallbackContext<TCallback>> callbackContext(reinterpret_cast<CallbackContext<TCallback>*>(context));
    if (!callbackContext->callback) return;

    callbackContext->callback(rc, callbackContext->appContext);
}

inline void CoreImpl::callbackNotificationWrapper(NVGSDK_NotificationType type, NVGSDK_Notification const* data, void* context)
{
    CallbackNotificationContext* callbackContext(reinterpret_cast<CallbackNotificationContext*>(context));
    if (!callbackContext->callback) return;

    switch (type)
    {
    case NVGSDK_NOTIFICATION_PERMISSIONS_CHANGED:
    {
        PermissionsChangedNotification notification;
        notification.context = callbackContext->appContext;
        for (size_t i = 0; i < data->permissionsChanged.scopePermissionTableSize; ++i)
        {
            notification.scopePermissions[data->permissionsChanged.scopePermissionTable[i].scope] = data->permissionsChanged.scopePermissionTable[i].permission;
        }
        callbackContext->callback(type, notification);
        break;
    }
    case NVGSDK_NOTIFICATION_OVERLAY_STATE_CHANGED:
    {
        OverlayStateChangedNotification notification;
        notification.context = callbackContext->appContext;
        notification.open = data->overlayStateChanged.open;
        notification.state = data->overlayStateChanged.state;
        callbackContext->callback(type, notification);
        break;
    }
    default:
        break;
    }
}

inline void CoreImpl::translateLocaleTable(std::map<std::string, std::string> const& localeMap, std::vector<NVGSDK_LocalizedPair>& pairs)
{
    for (auto it = localeMap.begin(); it != localeMap.end(); ++it)
    {
        pairs.push_back({
            it->first.c_str(),
            it->second.c_str()
        });
    }
}
}

#endif //GFESDK_ISDK_CPP_H
