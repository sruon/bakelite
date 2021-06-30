#pragma once
#include <gfesdk/bindings/cpp/highlights/highlights_types_cpp.h>
typedef NVGSDK_RetCode(__stdcall* NVGSDK_Create)(
    NVGSDK_HANDLE**,
    NVGSDK_CreateInputParams const*,
    NVGSDK_CreateResponse*);