#ifndef __UTILS_UTILS_HPP__
#define __UTILS_UTILS_HPP__

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <tuple>
#include <array>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <stdarg.h>

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

#define GHL_WINDOW_LAYER_NAME "Window"
#define GHL_DEBUG_LAYER_NAME "Debug"

namespace ghl {

}

#endif