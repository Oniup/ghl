#ifndef __UTILS_UTILS_HPP__
#define __UTILS_UTILS_HPP__

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <array>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <stdarg.h>

#if defined(_WIN32) && defined(GHL_BUILD_DLL)
#   define GHL_API __declspec(dllexport)
#elif defined(_WIN32) && defined(GHL_DLL)
#   define GHL_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(GHL_BUILD_DLL)
#   define GHL_API __attribute__((visibility("default")))
#else
#   define GHL_API
#endif

#ifndef NDEBUG
#   define GHL_ASSERT(CONDITION, ERR_MESSAGE)\
        if (CONDITION) {\
            std::printf(std::string("[ghl err]: " + std::string(ERR_MESSAGE)).c_str());\
            std::cin.get();\
            std::exit(-1);\
        }\

#   define GHL_ASSERT_FMT(CONDITION, ERR_FORMAT, ...)\
        if (CONDITION) {\
            std::printf(std::string("[ghl err]: " + std::string(ERR_FORMAT)).c_str() , __VA_ARGS__);\
            std::cin.get();\
            std::exit(-1);\
        }\

#else
#   define GHL_ASSERT(CONDITION, ERR_MESSAGE)
#   define GHL_ASSERT_FMT(CONDITION, ERR_FORMAT, ...)

#endif

namespace ghl {

}

#endif