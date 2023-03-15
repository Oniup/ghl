#ifndef __OGL_CORE_DEBUG_HPP__
#define __OGL_CORE_DEBUG_HPP__

#include "ogl/core/application_layer.hpp"
#include "ogl/utils/utils.hpp"

namespace ogl {

    enum DebugType {
        DebugType_Message = 0,
        DebugType_Warning,
        DebugType_Error,
    };

    class Debug : public ogl::ApplicationLayer {
    public:
        Debug();
        virtual ~Debug() override = default;

        static void log(std::string_view message, DebugType type = DebugType_Message);

        virtual void on_update() override;

    private:
        static Debug* m_instance;

        std::vector<std::tuple<DebugType, std::string>> m_logs{};
    };

}

#endif