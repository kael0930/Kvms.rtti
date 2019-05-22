#ifndef Kvms_h__
#define Kvms_h__

#include "KvGlobal.h"

#if defined(KVAPI_LIBRARY)
#  define KV_CORE_EXPORT K_DECL_EXPORT
#else
#  define KV_CORE_EXPORT K_DECL_IMPORT
#endif

#endif // Kvms_h__
