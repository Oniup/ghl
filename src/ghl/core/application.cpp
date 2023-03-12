#include "ghl/core/application.hpp"
#include "ghl/core/scene_manager.hpp"
#include "ghl/core/asset_handler.hpp"
#include "ghl/renderer/pipeline.hpp"

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
        for (ApplicationLayer* layer : m_layers) {
            if (layer->get_name() == name) {
                return layer;
            }
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
        Window* window = static_cast<Window*>(get_layer(GHL_RENDERER_WINDOW_LAYER_NAME));

        while (!window->should_close()) {
            window->clear_screen();

            for (ApplicationLayer* layer : m_layers) {
                layer->on_update();
            }

            window->swap_buffers();
        }
    }

    void Application::_init_required_layers() {
        push_layer(new Window(600, 600, "Test Window"));
        push_layer(new Pipeline());
        push_layer(new SceneManager());
        push_layer(new AssetHandler());
    }

}