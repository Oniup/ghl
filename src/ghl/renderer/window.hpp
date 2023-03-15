#ifndef __GHL_RENDERER_WINDOW_HPP__
#define __GHL_RENDERER_WINDOW_HPP__

#include "ghl/core/application_layer.hpp"
#include "ghl/utils/utils.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ghl {

    class Window : public ApplicationLayer {
    public:
        Window(int width, int height, std::string_view title);
        virtual ~Window() override;

        inline int get_width() const { return m_width;  }
        inline int get_height() const { return m_height; }
        inline const std::string& get_title() const { return m_title; }
        inline GLFWwindow* get_internal() { return m_internal; }
        inline const GLFWwindow* get_internal() const { return m_internal; }
        inline void set_clear_color(glm::vec4 clear_color) { m_clear_color = clear_color; }

        static void terminate_glfw_instance();

        void clear_screen();
        void swap_buffers();
        bool should_close();

    private:
        void _init_internal_window();

        static bool m_glfw_init;
        int m_width{};
        int m_height{};
        glm::vec4 m_clear_color{ 0.0f, 0.0f, 0.0f, 1.0f };
        std::string m_title{};
        GLFWwindow* m_internal{ nullptr };
    };

}

#endif