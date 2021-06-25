//
// GFE3 product version: 
//   - A.B.XY.N, if X > 0
//   - A.B.Y.N, otherwise
//
// A - Major release version.
// B - Minor release version.
// X - _dev to _rel_next rebase counter.
// Y - _rel_next to _rel rebase counter.
// N - Build counter.
//
#define NVIDIA_GFE3_VERSION_MAJOR 4
#define NVIDIA_GFE3_VERSION_MINOR 0
#define NVIDIA_GFE3_VERSION_DEV_REBASE 0
#define NVIDIA_GFE3_VERSION_RNEXT_REBASE 0
#define NVIDIA_GFE3_VERSION_BUILD 168

//
// Composite version values. We don't concatenate them here automatically because
//    (a) one can't use L-based wchar_t conversion with preprocessor-concatenated terminals; and
//    (b) some build scripts rely upon the explicit value for NVIDIA_GFE3_VERSION / NVIDIA_GFE3_VERSION_STRING_DOT
//

// Format: A,B,XY,N
#define NVIDIA_GFE3_VERSION          4,0,0,168
// Format: A.B.XY.N
#define NVIDIA_GFE3_VERSION_DOT      4.0.0.168
// Format: "A.B.XY.N"
#define NVIDIA_GFE3_VERSION_STRING   "4.0.0.168"

//
// Some common strings
//
#define NVIDIA_GFE3_PRODUCT_NAME     "NVIDIA GeForce Experience"

//
// These values are taken from nvVer.h
//
#define NV_COMPANY_NAME_STRING_SHORT "NVIDIA"
#define NV_COMPANY_NAME_STRING_FULL  "NVIDIA Corporation"
#define NV_COMPANY_NAME_STRING       NV_COMPANY_NAME_STRING_FULL
#define NV_COPYRIGHT_YEAR            "2020"
#define NV_COPYRIGHT                 "(C) " NV_COPYRIGHT_YEAR " " NV_COMPANY_NAME_STRING_FULL ". All rights reserved."  // Please do not use the non-ascii copyright symbol for (C).

