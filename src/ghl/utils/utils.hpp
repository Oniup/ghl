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
#include <initializer_list>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#define GHL_RENDERER_WINDOW_LAYER_NAME      "Window"
#define GHL_CORE_DEBUG_LAYER_NAME           "Debug"
#define GHL_CORE_SCENE_MANAGER_LAYER_NAME   "Scene Manager"
#define GHL_RENDERER_PIPELINE_LAYER_NAME    "Pipeline"
#define GHL_CORE_ASSET_HANDLER_NAME         "Asset Handler"

#define GHL_PIPELINE_RENDERER_BATCH_NAME    "Batch Renderer"
#define GHL_PIPELINE_RENDERER_INSTANCE_NAME "Instance Renderer" // TODO(Ewan): implement robust batch renderer before considering this and a way to use both

#ifndef NDEBUG
#   define GHL_ASSERT(CONDITION, ERR_MESSAGE)\
        if (CONDITION) {\
            std::printf(std::string("[ghl assert]: " + std::string(ERR_MESSAGE)).c_str());\
            std::cin.get();\
            std::exit(-1);\
        }\

#   define GHL_ASSERT_FMT(CONDITION, ERR_FORMAT, ...)\
        if (CONDITION) {\
            std::printf(std::string("[ghl assert]: " + std::string(ERR_FORMAT)).c_str() , __VA_ARGS__);\
            std::cin.get();\
            std::exit(-1);\
        }\

#else
#   define GHL_ASSERT(CONDITION, ERR_MESSAGE)
#   define GHL_ASSERT_FMT(CONDITION, ERR_FORMAT, ...)

#endif

#define GHL_REMOVE_ELEMENT_FROM_VECTOR_WITH_NAME(TYPE, STR_NAME, COMPARE_NAME, VECTOR)\
		for (std::vector<TYPE>::iterator it = VECTOR.begin(); it != VECTOR.end(); it++) {\
			if (it->COMPARE_NAME == STR_NAME) {\
				VECTOR.erase(it);\
				return;\
			}\
		}\

namespace ghl {
       
    inline void print_vec2(const glm::vec2& vec) {
        std::cout << vec.x << ", " << vec.y;
    }

    inline void print_vec3(const glm::vec3& vec) {
        std::cout << vec.x << ", " << vec.y << ", " << vec.z;
    }
    
    inline void print_vec4(const glm::vec4& vec) {
        std::cout << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
    }

}

#endif