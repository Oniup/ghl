#include "ogl/renderer/window.hpp"

#include <iostream>

namespace ogl {

    bool Window::m_glfw_init = false;

    Window::Window(int width, int height, std::string_view title) : ApplicationLayer(OGL_RENDERER_WINDOW_LAYER_NAME), m_width(width), m_height(height), m_title(title) {
        if (!m_glfw_init) {
            glfwInit();
        }
        _init_internal_window();
    }

    Window::~Window() {
        if (m_internal != nullptr) {
            glfwDestroyWindow(m_internal);
        }
    }

    void Window::terminate_glfw_instance() {
        glfwTerminate();
    }

    void Window::clear_screen() {
        glClearColor(m_clear_color.r, m_clear_color.g, m_clear_color.b, m_clear_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwGetFramebufferSize(m_internal, &m_width, &m_height);
        glViewport(0, 0, m_width, m_height);
    }

    void Window::swap_buffers() {
        glfwSwapBuffers(m_internal);
        glfwPollEvents();
    }

    bool Window::should_close() {
        return glfwWindowShouldClose(m_internal);
    }

    void Window::_init_internal_window() {
        if (m_internal != nullptr) {
            glfwDestroyWindow(m_internal);
        }

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 5);

        m_internal = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (m_internal == nullptr) {
            std::cout << "failed to create glfw window\n";
            glfwTerminate();
            std::exit(-1);
        }

        glfwMakeContextCurrent(m_internal);
        OGL_ASSERT(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Window::_init_internal_window() -> failed to initialize glad");
        glViewport(0, 0, m_width, m_height);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

}