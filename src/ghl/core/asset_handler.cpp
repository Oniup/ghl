#include "ghl/core/asset_handler.hpp"

namespace ghl {

	AssetHandler* AssetHandler::m_instance = nullptr;

	AssetHandler::AssetHandler() : ApplicationLayer(GHL_CORE_ASSET_HANDLER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "AssetHandler::AssetHandler() -> cannot created multiple asset handler application layers");

		m_instance = this;
	}

	AssetHandler::~AssetHandler() {
		for (Shader& shader : m_shaders) {
			Pipeline::destroy_shader_gpu_instance(shader);
		}

		for (Texture& texture : m_textures) {
			Pipeline::destroy_texture_gpu_instance(texture);
		}
	}

	Shader* AssetHandler::load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path) {
		m_shaders.push_back(Pipeline::load_shader_into_memory(name, vertex_path, fragment_path));
		return &m_shaders.back();
	}

	Texture* AssetHandler::load_texture_into_memory(std::string_view name, std::string_view path, bool flip) {
		m_textures.push_back(Pipeline::load_texture_into_memory(name, path, flip));
		return &m_textures.back();
	}

	Model* AssetHandler::load_model_into_memory(std::string_view name, std::string_view path) {
		return nullptr;
	}

	Shader* AssetHandler::get_shader(std::string_view name) {
		for (Shader& shader : m_shaders) {
			if (shader.name == name) {
				return &shader;
			}
		}

		return nullptr;
	}

	Texture* AssetHandler::get_texture(std::string_view name) {
		for (Texture& texture : m_textures) {
			if (texture.name == name) {
				return &texture;
			}
		}

		return nullptr;
	}

	Model* AssetHandler::get_model(std::string_view name) {
		return nullptr;
	}

	void AssetHandler::remove_shader(std::string_view name) {
		GHL_REMOVE_ELEMENT_FROM_VECTOR_WITH_NAME(Shader, name, name, m_shaders)
	}

	void AssetHandler::remove_texture(std::string_view name) {
		GHL_REMOVE_ELEMENT_FROM_VECTOR_WITH_NAME(Texture, name, name, m_textures)
	}

	void AssetHandler::remove_model(std::string_view name) {
		GHL_REMOVE_ELEMENT_FROM_VECTOR_WITH_NAME(Model, name, name, m_models);
	}

}