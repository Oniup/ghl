#include "ghl/renderer/pipeline.hpp"
#include "ghl/utils/file_system.hpp"
#include "ghl/core/debug.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <stb/stb_image.h>
#include <GLFW/glfw3.h>

namespace ghl {

	Pipeline* Pipeline::m_instance = nullptr;

	Pipeline::Pipeline() : ApplicationLayer(GHL_PIPELINE_LAYER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "Pipeline::Pipeline() -> cannot have mutiple pipeline instances loaded");

		m_instance = this;
	}

	Pipeline::~Pipeline() {
		for (Shader& shader : m_shaders) {
			glDeleteProgram(shader.id);
		}

		for (Texture& texture : m_textures) {
			glDeleteTextures(1, &texture.id);
		}
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

	Shader* Pipeline::load_shader(std::string_view name, std::string_view vertex_path, std::string_view fragment_path) {
		const char* paths[] = { fragment_path.data(), vertex_path.data() };
		uint32_t shaders[2] = { 0, 0 };

		for (size_t i = 0; i < 2; i++) {
			File file = File(paths[i], FileOpenState_ReadBinary);
			if (!file.exists()) {
				if (i == 1) {
					glDeleteShader(shaders[0]);
				}
				return nullptr;
			}

			std::string source = file.get_source();
			const char* src = source.c_str();
			uint32_t shader = static_cast<uint32_t>(glCreateShader(GL_FRAGMENT_SHADER + i));
			glShaderSource(shader, 1, static_cast<const char* const*>(&src), nullptr);
			glCompileShader(shader);

			int result;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
			if (!result) {
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
				char* message = static_cast<char*>(std::malloc(sizeof(char*) * result));
				glGetShaderInfoLog(shader, result, &result, message);
				message[result] = '\0';

				Debug::log(std::string("Pipeline::load_shader(std::string_view, std::string_view, std::string_view) -> failed to compile shader: ") + message, DebugType_Error);
				std::free(message);

				if (i == 1) {
					glDeleteShader(shaders[0]);
				}
			}

			shaders[i] = shader;
		}

		uint32_t program = static_cast<uint32_t>(glCreateProgram());
		glAttachShader(program, shaders[0]);
		glAttachShader(program, shaders[1]);
		glLinkProgram(program);

		glDeleteShader(shaders[0]);
		glDeleteShader(shaders[1]);

		int result;
		glGetProgramiv(program, GL_LINK_STATUS, &result);
		if (!result) {
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
			char* message = static_cast<char*>(std::malloc(sizeof(char*) * result));
			glGetProgramInfoLog(program, result, &result, message);
			message[result] = '\0';

			Debug::log(std::string("Pipeline::load_shader(std::string_view, std::string_view, std::string_view) -> failed to link shader programs") + message, DebugType_Error);
			std::free(message);

			return nullptr;
		}

		m_shaders.push_back({ std::string(name), program });
		return &m_shaders.back();
	}

	Texture* Pipeline::load_texture(std::string_view name, std::string_view path, bool flip) {
		stbi_set_flip_vertically_on_load(flip);

		int width, height, channels;
		uint8_t* buffer = stbi_load(path.data(), &width, &height, &channels, 4);
		if (buffer == nullptr) {
			Debug::log("Pipeline::load_texture(std::string_view, std::string_view, bool) -> failed to find file \"" + std::string(path.data()) + "\"", DebugType_Error);
			return nullptr;
		}

		uint32_t format = 0;
		switch (channels) {
		case 0:
			Debug::log("Pipeline::load_texture(std::string_view, std::string_view, bool) -> channel format is set to 0 for \"" + std::string(path.data()) + "\"", DebugType_Error);
			return nullptr;
		case 1: format = GL_RED;	break;
		case 2: format = GL_RG;		break;
		case 3: format = GL_RGB;	break;
		case 4: format = GL_RGBA;	break;
		}

		uint32_t texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, buffer);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_textures.push_back({ name.data(), glm::ivec2(width, height), texture });
		return &m_textures.back();
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