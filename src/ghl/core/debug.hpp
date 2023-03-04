#ifndef __GHL_CORE_DEBUG_HPP__
#define __GHL_CORE_DEBUG_HPP__

#include "ghl/core/application_layer.hpp"
#include "ghl/utils/utils.hpp"

namespace ghl {

    enum DebugType {
        DebugType_Message = 0,
        DebugType_Warning,
        DebugType_Error,
    };

    class Debug : public ghl::ApplicationLayer {
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