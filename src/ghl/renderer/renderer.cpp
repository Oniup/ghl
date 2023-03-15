#include "ghl/renderer/renderer.hpp"
#include "ghl/renderer/renderer_comps.hpp"
#include "ghl/core/asset_handler.hpp"
#include "ghl/renderer/window.hpp"
#include "ghl/core/debug.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace ghl {

	BasicRenderer::BasicRenderer() : PipelineRenderer(GHL_PIPELINE_RENDERER_BATCH_NAME) {
		SceneManager::get()->push_system<BasicRenderer::_CollectData>(this);

		m_vertex_array.bind();
		m_vertex_array.set_attribute(3, sizeof(Vertex), 0);
		m_vertex_array.set_attribute(3, sizeof(Vertex), offsetof(Vertex, normal));
		m_vertex_array.set_attribute(2, sizeof(Vertex), offsetof(Vertex, uv));
		m_vertex_array.unbind();
	}

	void BasicRenderer::on_render() {
		for (_RenderPass& pass : m_render_passes) {
			Mesh* mesh = pass.mesh;
			Material* material = pass.material;

			m_vertex_array.bind();

			m_vertex_array.set_vertex_data(mesh->vertices.size(), sizeof(Vertex), &mesh->vertices[0]);
			m_vertex_array.set_index_data(mesh->indices.size(), &mesh->indices[0]);

			// TODO(Ewan): get from camera
			Window* window = Pipeline::get()->get_window();
			static glm::mat4 projection_matrix = glm::perspective(
				glm::radians(45.0f), static_cast<float>(window->get_width()) / static_cast<float>(window->get_height()), 0.1f, 100.0f
			);

			glUseProgram(material->shader->program);
			glUniformMatrix4fv(glGetUniformLocation(material->shader->program, "u_model_matrix"), 1, GL_FALSE, &pass.transforms[0][0]);
			glUniformMatrix4fv(glGetUniformLocation(material->shader->program, "u_projection_matrix"), 1, GL_FALSE, &projection_matrix[0][0]);

			glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, (void*)0);
			m_vertex_array.unbind();
			glUseProgram(0);
		}
	}

	void BasicRenderer::submit(Mesh* mesh, const glm::mat4& model_matrix) {
		Material* material = nullptr;
		if (mesh->material != nullptr) {
			// TODO(Ewan): add this after being able to render the cube
			material = mesh->material;
			if (material->shader == nullptr) {
				material->shader = AssetHandler::get()->get_shader(GHL_ASSET_MATERIAL_PHONG_DEFAULT_NAME);
			}
		}
		else {
			material = AssetHandler::get()->get_material(GHL_ASSET_MATERIAL_PHONG_DEFAULT_NAME);
		}
		m_render_passes.push_back({ material, mesh, model_matrix });
	}

	BasicRenderer::_CollectData::_CollectData(BasicRenderer* renderer) : m_renderer(renderer) {

	}

	void BasicRenderer::_CollectData::on_before_render(entt::registry& registry) {
		auto group = registry.group<TransformComponent>(entt::get<ModelRendererComponent>);
		for (auto entity : group) {
			TransformComponent& transform = group.get<TransformComponent>(entity);
			ModelRendererComponent& model = group.get<ModelRendererComponent>(entity);

			glm::mat4 model_matrix = glm::mat4(1.0f);
			model_matrix = glm::translate(model_matrix, transform.position);
			model_matrix = glm::scale(model_matrix, transform.scale);
			model_matrix = glm::rotate(model_matrix, glm::radians(transform.rotation.w), glm::vec3(transform.rotation));

			for (Mesh& mesh : model.model->meshes) {
				m_renderer->submit(&mesh, model_matrix);
			}
		}
	}

}
