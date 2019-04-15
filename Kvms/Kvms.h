#ifndef Kvms_h__
#define Kvms_h__

#define K_DECL_EXPORT     __declspec(dllexport)
#define K_DECL_IMPORT     __declspec(dllimport)

#if defined(KVAPI_LIBRARY)
#  define KV_CORE_EXPORT K_DECL_EXPORT
#else
#  define KV_CORE_EXPORT K_DECL_IMPORT
#endif

#endif // Kvms_h__
