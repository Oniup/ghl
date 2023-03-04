#ifndef __GHL_RENDERER_WINDOW_HPP__
#define __GHL_RENDERER_WINDOW_HPP__

#include "ghl/core/application_layer.hpp"
#include "ghl/utils/utils.hpp"

#include <GLFW/glfw3.h>

namespace ghl {

    class Window : public ApplicationLayer {
    public:
        Window(int width, int height, std::string_view title);
        virtual ~Window() override;

        inline GLFWwindow* get_internal() { return m_internal; }
        inline const GLFWwindow* get_internal() const { return m_internal; }

        static void terminate_glfw_instance();

        void swap_buffers();
        bool should_close();

    private:
        void _init_internal_window();

        static bool m_glfw_init;
        int m_width{};
        int m_height{};
        std::string m_title{};
        GLFWwindow* m_internal{ nullptr };
    };

}

#endif