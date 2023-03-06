#include "ghl/core/debug.hpp"

namespace ghl {

    Debug* Debug::m_instance = nullptr;

    Debug::Debug() : ApplicationLayer(GHL_DEBUG_LAYER_NAME) {
        GHL_ASSERT(m_instance != nullptr, "Debug::Debug() -> cannot have multiple debug layers");

        m_instance = this;
    }

    void Debug::log(std::string_view message, DebugType type) {
        if (m_instance != nullptr) {
            m_instance->m_logs.push_back(std::tuple<DebugType, std::string>(type, message));
        }
    }

    void Debug::on_update() {
        for (std::tuple<DebugType, std::string>& debug_log : m_logs) {
            switch(std::get<DebugType>(debug_log)) {
            case DebugType_Message:
                std::cout << "[debug layer message]: ";
                break;
            case DebugType_Warning:
                std::cout << "[debug layer warning]: ";
                break;
            case DebugType_Error:
                std::cout << "[debug layer error]: ";
                break;
            }

            std::cout << std::get<std::string>(debug_log) << "\n";
        }

        m_logs.clear();
    }

}