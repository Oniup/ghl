#include "ghl/core/application.hpp"

namespace ghl {

    Application::Application() {
        _init_required_layers();
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
        GHL_ASSERT(layer != nullptr, "ApplicationLayer::push_layer -> layer == nullptr")

        m_layers.insert(m_layers.begin(), layer);
        return m_layers.front();
    }

    void Application::run() {
        std::cout << "application is running\n";
    }

    void Application::_init_required_layers() {
        std::cout << "init required layers\n";
    }

}