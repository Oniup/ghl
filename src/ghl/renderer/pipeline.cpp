#include "ghl/renderer/pipeline.hpp"
#include "ghl/utils/file_system.hpp"
#include "ghl/core/debug.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
		const char* paths[] = { fragment_path.data(), vertex_path.data() };
		uint32_t shaders[2];

		for (size_t i = 0; i < 2; i++) {
			File file = File(paths[i], FileOpenState_ReadBinary);
			if (!file.exists()) {
				if (i == 1) {
					glDeleteShader(shaders[0]);
				}
				return nullptr;
			}

			std::string source = file.get_source();
			std::cout << "source \"" << paths[i] << "\"\n" << source << "\n";
		}

		return nullptr;
	}

	Texture* Pipeline::load_texture(std::string_view path, int channels) {
		return nullptr;
	}

	void Pipeline::remove_shader(std::string_view name) {
		for (std::vector<Shader>::iterator shader = m_shaders.begin(); shader != m_shaders.end(); shader++) {
			if (shader->name == name) {
				m_shaders.erase(shader);
				return;
			}
		}
	}

	void Pipeline::remove_texture(std::string_view name) {
		for (std::vector<Texture>::iterator texture = m_textures.begin(); texture != m_textures.end(); texture++) {
			if (texture->name == name) {
				m_textures.erase(texture);
				return;
			}
		}
	}

}