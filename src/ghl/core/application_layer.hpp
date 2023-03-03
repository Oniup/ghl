#ifndef __GHL_CORE_APPLICATION_LAYER_HPP__
#define __GHL_CORE_APPLICATION_LAYER_HPP__

#include "ghl/utils/utils.hpp"

namespace ghl {

    class GHL_API ApplicationLayer {
    public:
        ApplicationLayer(std::string_view name) : m_name(name) {}
        virtual ~ApplicationLayer() = default;

        const std::string& get_name() const { return m_name; }

        virtual void on_update() = 0;
    private:
        std::string m_name{};
    };

}

#endif