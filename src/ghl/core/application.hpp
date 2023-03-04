#ifndef __GHL_CORE_APPLICATION_HPP__
#define __GHL_CORE_APPLICATION_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"
#include "ghl/renderer/window.hpp"
#include "ghl/core/debug.hpp"

namespace ghl {

    class Application {
    public:
        Application();
        virtual ~Application();

        ApplicationLayer* get_layer(std::string_view name);
        Window* get_window_layer() { return m_window_layer; }
        const ApplicationLayer* get_layer(std::string_view name) const;
        const Window* get_window_layer() const { return m_window_layer; }

        ApplicationLayer* push_layer(ApplicationLayer* layer);

        void run();
    private:
        void _init_required_layers();

        Window* m_window_layer{ nullptr };
        std::vector<ApplicationLayer*> m_layers{};
    };

}

#endif