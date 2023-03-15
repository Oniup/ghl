#ifndef __OGL_CORE_APPLICATION_LAYER_HPP__
#define __OGL_CORE_APPLICATION_LAYER_HPP__

#include "ogl/utils/utils.hpp"

namespace ogl {

    class ApplicationLayer {
    public:
        ApplicationLayer(std::string_view name) : m_name(name) {}
        virtual ~ApplicationLayer() = default;

        const std::string& get_name() const { return m_name; }

        virtual void on_update() {};
    private:
        std::string m_name{};
    };

}

#endif