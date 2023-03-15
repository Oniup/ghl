#include "ogl/core/application.hpp"
#include "ogl/core/scene_manager.hpp"
#include "ogl/core/asset_handler.hpp"
#include "ogl/renderer/pipeline.hpp"

namespace ogl {

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
        OGL_ASSERT(layer == nullptr, "ApplicationLayer::push_layer -> layer == nullptr")

        m_layers.insert(m_layers.begin(), layer);
        return m_layers.front();
    }

    void Application::run() {
        Window* window = static_cast<Window*>(get_layer(OGL_RENDERER_WINDOW_LAYER_NAME));

        while (!window->should_close()) {
            window->clear_screen();

            for (ApplicationLayer* layer : m_layers) {
                layer->on_update();
            }

            window->swap_buffers();
        }
    }

    void Application::_init_required_layers() {
        Window* window_layer = static_cast<Window*>(push_layer(new Window(600, 600, "Test Window")));
        push_layer(new Pipeline(window_layer));
        push_layer(new SceneManager());
        push_layer(new AssetHandler());
    }

}