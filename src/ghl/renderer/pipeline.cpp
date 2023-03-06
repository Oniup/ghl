#include "ghl/renderer/pipeline.hpp"

namespace ghl {

	Pipeline* Pipeline::m_instance = nullptr;

	Pipeline::Pipeline() : ApplicationLayer(GHL_PIPELINE_LAYER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "Pipeline::Pipeline() -> cannot have mutiple pipeline instances loaded");

		m_instance = this;
	}

	void Pipeline::on_update() {

	}

	Shader* Pipeline::get_shader(std::string_view name) {
		for (Shader& shader : m_shaders) {
			if (shader.name == name) {
				return &shader;
			}
		}

		return nullptr;
	}

	Texture* Pipeline::get_texture(std::string_view name) {
		for (Texture& texture : m_textures) {
			if (texture.name == name) {
				return &texture;
			}
		}

		return nullptr;
	}

	Shader* Pipeline::load_shader(std::string_view vertex_path, std::string_view fragment_path) {
		return nullptr;
	}

	Texture* Pipeline::load_texture(std::string_view path, int channels) {
		return nullptr;
	}

	void Pipeline::remove_shader(std::string_view name) {

	}

	void Pipeline::remove_texture(std::string_view name) {

	}

}