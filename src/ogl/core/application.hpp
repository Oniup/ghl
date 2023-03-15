#ifndef __OGL_CORE_APPLICATION_HPP__
#define __OGL_CORE_APPLICATION_HPP__

#include "ogl/utils/utils.hpp"
#include "ogl/core/application_layer.hpp"
#include "ogl/renderer/window.hpp"
#include "ogl/core/debug.hpp"

namespace ogl {

    class Application {
    public:
        Application();
        virtual ~Application();

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