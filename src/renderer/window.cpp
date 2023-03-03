#include "ghl/renderer/window.hpp"

#include <iostream>

namespace ghl {


    Window::Window(int width, int height, std::string_view title) : m_width(width), m_height(height), m_title(title) {
        _init_internal_window();
    }

    Window::~Window() {
        if (m_internal != nullptr) {
            glfwDestroyWindow(m_internal);
        }
    }

    void Window::init_glfw() {
        glfwInit();
    }

    void Window::terminate_glfw() {
        glfwTerminate();
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

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 5);

        m_internal = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
        if (m_internal == nullptr) {
            std::cout << "failed to create glfw window\n";
            std::exit(-1);
        }

        glfwMakeContextCurrent(m_internal);
    }

}