#ifndef __UTILS_SHARED_HPP__
#define __UTILS_SHARED_HPP__

#if defined(_WIN32) && defined(GHL_BUILD_DLL)
#   define GHL_API __declspec(dllexport)
#elif defined(_WIN32) && defined(GHL_DLL)
#   define GHL_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(GHL_BUILD_DLL)
#   define GHL_API __attribute__((visibility("default")))
#else
#   define GHL_API
#endif

#endif