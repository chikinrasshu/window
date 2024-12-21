#pragma once

#if defined _WIN32 || defined __CYGWIN__
#    define CHK_WINDOW_HELPER_DLL_IMPORT __declspec(dllimport)
#    define CHK_WINDOW_HELPER_DLL_EXPORT __declspec(dllexport)
#    define CHK_WINDOW_HELPER_DLL_LOCAL
#else
#    if __GNUC__ >= 4
#        define CHK_WINDOW_HELPER_DLL_IMPORT                                   \
            __attribute__((visibility("default")))
#        define CHK_WINDOW_HELPER_DLL_EXPORT                                   \
            __attribute__((visibility("default")))
#        define CHK_WINDOW_HELPER_DLL_LOCAL                                    \
            __attribute__((visibility("hidden")))
#    else
#        define CHK_WINDOW_HELPER_DLL_IMPORT
#        define CHK_WINDOW_HELPER_DLL_EXPORT
#        define CHK_WINDOW_HELPER_DLL_LOCAL
#    endif
#endif

#ifdef CHK_WINDOW_DLL
#    ifdef CHK_WINDOW_DLL_EXPORTS
#        define CHK_WINDOW_API CHK_WINDOW_HELPER_DLL_EXPORT
#    else
#        define CHK_WINDOW_API CHK_WINDOW_HELPER_DLL_IMPORT
#    endif
#    define CHK_WINDOW_LOCAL CHK_WINDOW_HELPER_DLL_LOCAL
#else
#    define CHK_WINDOW_API
#    define CHK_WINDOW_LOCAL
#endif
