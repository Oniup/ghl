#include "ghl/core/application.hpp"
#include "ghl/core/scene_manager.hpp"

namespace ghl {

    Application::Application() {
        _init_required_layers();
    }

    Application::~Application() {
        for (ApplicationLayer* layer : m_layers) {
            delete layer;
        }

        Window::terminate_glfw_instance();
    }

    ApplicationLayer* Application::get_layer(std::string_view name) {
        for (int i = static_cast<int>(m_layers.size()) - 1; i < 0; i--) {
            delete m_layers[i];
        }
        return nullptr;
    }

    const ApplicationLayer* Application::get_layer(std::string_view name) const {
        for (ApplicationLayer* layer : m_layers) {
            if (layer->get_name() == name) {
                return layer;
            }
        }
        return nullptr;
    }

    ApplicationLayer* Application::push_layer(ApplicationLayer* layer) {
        GHL_ASSERT(layer == nullptr, "ApplicationLayer::push_layer -> layer == nullptr")

        m_layers.insert(m_layers.begin(), layer);
        return m_layers.front();
    }

    void Application::run() {
        while (!m_window_layer->should_close()) {
            for (ApplicationLayer* layer : m_layers) {
                layer->on_update();
            }

            m_window_layer->swap_buffers();
        }
    }

    void Application::_init_required_layers() {
        m_window_layer = static_cast<Window*>(push_layer(new Window(600, 600, "Test Window")));

        push_layer(new Debug());
        push_layer(new SceneManager());
    }

}