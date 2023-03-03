#ifndef __GHL_CORE_APPLICATION_HPP__
#define __GHL_CORE_APPLICATION_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/renderer/window.hpp"
#include "ghl/core/application_layer.hpp"

namespace ghl {

    class GHL_API Application {
    public:
        Application();
        virtual ~Application() = default;

        ApplicationLayer* get_layer(std::string_view name);
        const ApplicationLayer* get_layer(std::string_view name) const;

        ApplicationLayer* push_layer(ApplicationLayer* layer);

        void run();
    private:
        void _init_required_layers();

        std::vector<ApplicationLayer*> m_layers{};
    };

}

#endif