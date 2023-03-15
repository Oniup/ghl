#include "ghl/renderer/pipeline.hpp"
#include "ghl/utils/file_system.hpp"
#include "ghl/core/debug.hpp"
#include "ghl/core/scene_manager.hpp"
#include "ghl/renderer/renderer_comps.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace ghl {

	Pipeline* Pipeline::m_instance = nullptr;

	VertexArrayBuffer::VertexArrayBuffer() : VertexArrayBuffer(true, 0, 0, nullptr, 0, nullptr) {
	}

	VertexArrayBuffer::VertexArrayBuffer(VertexArrayBuffer&& other) noexcept {
		*this = std::move(other);
	}

	VertexArrayBuffer::VertexArrayBuffer(const VertexArrayBuffer& other) {
		*this = other;
	}

	VertexArrayBuffer::VertexArrayBuffer(bool is_dynamic, size_t vertex_data_size, size_t vertex_data_type_size, const void* vertex_data, size_t element_data_size, const uint32_t* element_data) : m_is_dynamic(is_dynamic) {
		glGenVertexArrays(1, &m_vertex_array);
		glBindVertexArray(m_vertex_array);
		glGenBuffers(1, &m_vertex_buffer);
		glGenBuffers(1, &m_index_buffer);

		int draw_state = m_is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
		glBufferData(GL_ARRAY_BUFFER, vertex_data_type_size * vertex_data_size, vertex_data, draw_state);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * element_data_size, element_data, draw_state);

		m_vertex_buffer_size = vertex_data_size;
		m_index_buffer_size = element_data_size;
	}

	VertexArrayBuffer::~VertexArrayBuffer() {
		if (m_vertex_array != 0) {
			glDeleteBuffers(1, &m_vertex_buffer);
			glDeleteBuffers(1, &m_index_buffer);
			glDeleteVertexArrays(1, &m_vertex_array); 
		}
	}

	VertexArrayBuffer& VertexArrayBuffer::operator=(VertexArrayBuffer&& other) noexcept {
		m_vertex_array = other.m_vertex_array;
		m_vertex_buffer = other.m_vertex_buffer;
		m_index_buffer = other.m_index_buffer;
		m_vertex_buffer_size = other.m_vertex_buffer_size;
		m_index_buffer_size = other.m_index_buffer_size;
		m_is_dynamic = other.m_is_dynamic;
		m_attribute_count = other.m_attribute_count;

		other.m_vertex_array = 0;
		other.m_vertex_buffer = 0;
		other.m_index_buffer = 0;
		other.m_vertex_buffer_size = 0;
		other.m_index_buffer_size = 0;
		other.m_is_dynamic = 0;
		other.m_attribute_count = 0;
		
		return *this;
	}

	VertexArrayBuffer& VertexArrayBuffer::operator=(const VertexArrayBuffer& other) {
		if (m_vertex_array != 0) {
			glDeleteBuffers(1, &m_vertex_buffer);
			glDeleteBuffers(1, &m_index_buffer);
			glDeleteVertexArrays(1, &m_vertex_array); 
		}

		m_vertex_array = other.m_vertex_array;
		m_vertex_buffer = other.m_vertex_buffer;
		m_index_buffer = other.m_index_buffer;
		m_vertex_buffer_size = other.m_vertex_buffer_size;
		m_index_buffer_size = other.m_index_buffer_size;
		m_is_dynamic = other.m_is_dynamic;
		m_attribute_count = other.m_attribute_count;
		
		return *this;
	}

	void VertexArrayBuffer::set_attribute(uint32_t element_count, uint32_t stride, int offset) {
		glEnableVertexAttribArray(m_attribute_count);
		glVertexAttribPointer(m_attribute_count, element_count, GL_FLOAT, GL_FALSE, stride, (const void*)offset);
		m_attribute_count++;
	}

	void VertexArrayBuffer::set_vertex_data(size_t vertices_size, size_t vertex_type_size, const void* vertices, uint32_t offset) {
		glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);

		if (m_vertex_buffer_size == vertices_size) {
			glBufferSubData(GL_ARRAY_BUFFER, offset, vertex_type_size * vertices_size, vertices);
		}
		else {
			glBufferData(GL_ARRAY_BUFFER, vertex_type_size * vertices_size, vertices, m_is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			m_vertex_buffer_size = vertices_size;
		}
	}

	void VertexArrayBuffer::set_index_data(size_t indices_size, const uint32_t* indices, uint32_t offset) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

		if (m_vertex_buffer_size == indices_size) {
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, sizeof(uint32_t) * indices_size, indices);
		}
		else {
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_size, indices, m_is_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
			m_index_buffer_size = indices_size;
		}
	}

	void VertexArrayBuffer::bind() {
		glBindVertexArray(m_vertex_array);
	}

	void VertexArrayBuffer::unbind() {
		glBindVertexArray(0);
	}

	Pipeline::Pipeline(Window* window_layer) : ApplicationLayer(GHL_RENDERER_PIPELINE_LAYER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "Pipeline::Pipeline() -> cannot have mutiple pipeline instances loaded");

		m_instance = this;
		m_window_layer = window_layer;
	}

	Pipeline::~Pipeline() {
		for (PipelineRenderer* renderer : m_renderers) {
			delete renderer;
		}
	}

	void Pipeline::push_renderer(PipelineRenderer* renderer) {
		if (renderer == nullptr) {
			Debug::log("Pipeline::push_renderer(PipelineRenderer*) -> renderer == nullptr");
			return;
		}
		m_renderers.push_back(renderer);
	}

	PipelineRenderer* Pipeline::get_renderer(std::string_view name) {
		for (PipelineRenderer* renderer : m_renderers) {
			if (renderer->get_name() == name) {
				return renderer;
			}
		}
		return nullptr;
	}

	void Pipeline::remove_renderer(std::string_view name) {
		for (size_t i = 0; i < m_renderers.size(); i++) {
			if (m_renderers[i]->get_name() == name) {
				delete m_renderers[i];
				m_renderers.erase(m_renderers.begin() + i);
			}
		}
	}

	void Pipeline::on_update() {
		for (PipelineRenderer* renderer : m_renderers) {
			renderer->on_render();
		}
	}

	Shader Pipeline::load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path) {
		const char* paths[] = { fragment_path.data(), vertex_path.data() };
		uint32_t shaders[2] = { 0, 0 };

		for (size_t i = 0; i < 2; i++) {
			File file = File(paths[i], FileOpenState_ReadBinary);
			if (!file.exists()) {
				if (i == 1) {
					glDeleteShader(shaders[0]);
				}
				return {};
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

				return {};
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

			return {};
		}

		return { name.data(), program };
	}

	Texture Pipeline::load_texture_into_memory(std::string_view name, std::string_view path, bool flip) {
		stbi_set_flip_vertically_on_load(flip);

		int width, height, channels;
		uint8_t* buffer = stbi_load(path.data(), &width, &height, &channels, 4);
		if (buffer == nullptr) {
			Debug::log("Pipeline::load_texture(std::string_view, std::string_view, bool) -> failed to find file \"" + std::string(path.data()) + "\"", DebugType_Error);
			return {};
		}

		uint32_t format = 0;
		switch (channels) {
		case 0:
			Debug::log("Pipeline::load_texture(std::string_view, std::string_view, bool) -> channel format is set to 0 for \"" + std::string(path.data()) + "\"", DebugType_Error);
			return {};
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

		return { name.data(), glm::ivec2(width, height), texture };
	}

	void Pipeline::destroy_shader_resource(Shader* shader) {
		if (shader != nullptr) {
			if (shader->program != 0) {
				glDeleteShader(shader->program);
				shader->program = 0;
			}
		}
	}

	void Pipeline::destroy_texture_resource(Texture* texture) {
		if (texture != nullptr) {
			if (texture->image != 0) {
				glDeleteTextures(1, &texture->image);
				texture->image = 0;
			}
		}
	}

}