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

	Pipeline::Pipeline() : ApplicationLayer(GHL_RENDERER_PIPELINE_LAYER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "Pipeline::Pipeline() -> cannot have mutiple pipeline instances loaded");

		m_instance = this;
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

	void Pipeline::destroy_shader_gpu_instance(Shader& shader) {
		glDeleteProgram(shader.id);
	}

	void Pipeline::destroy_texture_gpu_instance(Texture& texture) {
		glDeleteTextures(1, &texture.id);
	}

	BatchRenderer::BatchRenderer() : PipelineRenderer(GHL_PIPELINE_RENDERER_BATCH_NAME) {
		SceneManager::get()->push_system<BatchRenderer::_BatchSystem>(this);
	}

	void BatchRenderer::on_render() {

	}

	void BatchRenderer::submit(Model::Mesh* mesh, glm::mat4& model_matrix, Material* material) {
		
	}

	BatchRenderer::_BatchSystem::_BatchSystem(BatchRenderer* renderer) : m_renderer(renderer) {

	}

	void BatchRenderer::_BatchSystem::on_render(entt::registry& registry) {
		if (m_renderer != nullptr) {
			std::cout << "has renderer reference\n";
		}
		else {
			std::cout << "doesn't have renderer reference\n";
		}

		auto group = registry.group<TransformComponent>(entt::get<ModelRendererComponent>);
		for (auto entity : group) {
			// TODO(Ewan): rework how model data is retrieved 
			TransformComponent& transform = group.get<TransformComponent>(entity);
			ModelRendererComponent& model = group.get<ModelRendererComponent>(entity);

			glm::mat4 model_matrix = glm::mat4(1.0f);
			model_matrix = glm::translate(model_matrix, transform.position);
			model_matrix = glm::scale(model_matrix, transform.scale);
			model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation.w), glm::vec3(transform.rotation));

			for (Model::Mesh& mesh : model.asset->meshes) {
				m_renderer->submit(&mesh, model_matrix, mesh.material);
			}
		}
	}

}