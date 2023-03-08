#ifndef __GHL_RENDERER_PIPELINE_HPP__
#define __GHL_RENDERER_PIPELINE_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"
#include "ghl/core/scene_manager.hpp"

namespace ghl {

	struct Vertex {
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 uv{};
	};

	struct Shader {
		~Shader() = default;

		std::string name{};
		uint32_t id{};
	};

	struct Texture {
		~Texture() = default;

		std::string name{};
		glm::ivec2 size{};
		uint32_t id{};
	};

	struct Material {
		~Material() = default;

		std::string name{};
		Shader* shader{ nullptr };
		std::vector<Texture*> textures{};
		float shininess{};
	};

	struct Model {
		struct Mesh {
			~Mesh() = default;

			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
			Material* material{ nullptr };
		};

		~Model() = default;

		std::string name{};
		std::vector<Mesh> meshes{};
	};

	class PipelineRenderer {
	public:
		inline PipelineRenderer(std::string_view name) : m_name(name) {}
		virtual ~PipelineRenderer() = default;

		inline const std::string& get_name() const { return m_name; }

		virtual void on_render() = 0;

	protected:
		std::vector<Vertex> m_vertices{};

	private:
		std::string m_name{};
	};

	class BatchRenderer : public PipelineRenderer {
	public:
		BatchRenderer();
		virtual ~BatchRenderer() override = default;

		virtual void on_render() override;
		void submit(Model::Mesh* mesh, glm::mat4& model_matrix, Material* material);

	private:
		struct _Batch {
			Shader* shader{};
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};
			std::vector<Material*> materials{};
			std::vector<glm::mat4> transforms{};
		};

		class _BatchSystem : public System {
		public:
			_BatchSystem(BatchRenderer* renderer);
			virtual void on_render(entt::registry& registry) override;

		private:
			BatchRenderer* m_renderer{ nullptr };
		};

		std::vector<_Batch> m_batches{};
	};

	class Pipeline : public ApplicationLayer {
	public:
		static inline Pipeline* get() { return m_instance; }

		static Shader load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		static Texture load_texture_into_memory(std::string_view name, std::string_view path, bool flip);

		static void destroy_shader_gpu_instance(Shader& shader);
		static void destroy_texture_gpu_instance(Texture& texture);

		Pipeline();
		virtual ~Pipeline() override;

		void push_renderer(PipelineRenderer* renderer);
		PipelineRenderer* get_renderer(std::string_view name);
		void remove_renderer(std::string_view name);

		virtual void on_update() override;

	private:
		static Pipeline* m_instance;

		std::vector<PipelineRenderer*> m_renderers{};
	};

}

#endif