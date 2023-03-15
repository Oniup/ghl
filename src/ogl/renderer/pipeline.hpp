#ifndef __OGL_RENDERER_PIPELINE_HPP__
#define __OGL_RENDERER_PIPELINE_HPP__

#include "ogl/utils/utils.hpp"
#include "ogl/core/application_layer.hpp"
#include "ogl/core/scene_manager.hpp"
#include "ogl/renderer/window.hpp"

namespace ogl {

	class VertexArrayBuffer {
	public:
		VertexArrayBuffer();
		VertexArrayBuffer(VertexArrayBuffer&& other) noexcept;
		// only use this for temporary copying, as they both share the same vertex array index
		VertexArrayBuffer(const VertexArrayBuffer& other);
		VertexArrayBuffer(bool is_dynamic, size_t vertex_data_size, size_t vertex_data_type_size, const void* vertex_data, size_t element_data_size, const uint32_t* element_data);
		~VertexArrayBuffer();

		VertexArrayBuffer& operator=(VertexArrayBuffer&& other) noexcept;
		// only use this for temporary copying, as they both share the same vertex array index
		VertexArrayBuffer& operator=(const VertexArrayBuffer& other);

		inline bool& is_dynamic() { return m_is_dynamic; }
		inline bool is_dynamic() const { return m_is_dynamic; }

		inline size_t get_vertex_buffer_size() const { return m_vertex_buffer_size; }
		inline size_t get_index_buffer_size() const { return m_index_buffer_size; }
		inline uint32_t get_vertex_array() const { return m_vertex_array; }
		inline uint32_t get_vertex_buffer() const { return m_vertex_buffer; }
		inline uint32_t get_element_buffer() const { return m_index_buffer; }
		inline uint32_t get_attribute_count() const { return m_attribute_count; }

		void set_attribute(uint32_t element_count, uint32_t stride, int offset);
		void set_vertex_data(size_t vertices_size, size_t vertex_type_size, const void* vertices, uint32_t offset = 0);
		void set_index_data(size_t indices_size, const uint32_t* indices, uint32_t offset = 0);

		void bind();
		void unbind();

	private:
		bool m_is_dynamic{ true };
		uint32_t m_vertex_array{};
		uint32_t m_vertex_buffer{};
		uint32_t m_index_buffer{};
		uint32_t m_attribute_count{};
		size_t m_vertex_buffer_size{};
		size_t m_index_buffer_size{};
	};

	struct Vertex {
		glm::vec3 position{};
		glm::vec3 normal{};
		glm::vec2 uv{};
	};

	struct Shader {
		std::string name{};
		uint32_t program{};

		~Shader() = default;
	};

	struct Texture {
		std::string name{};
		glm::ivec2 size{};
		uint32_t image{};

		~Texture() = default;
	};

	// TODO(Ewan): updated to support PBR rendering
	enum MaterialTextureType {
		MaterialTextureType_Diffuse = 0,
		MaterialTextureType_Specular,
		MaterialTextureType_Ambient,
	};

	struct Material {
		std::string name{};
		Shader* shader{ nullptr };
		std::vector<std::tuple<Texture*, MaterialTextureType>> textures{};
		float shininess{};

		~Material() = default;
	};

	// TODO(Ewan): this needs to change for release runtime application, for editing the game its fine
	enum ModelFileType {
		ModelFileType_Obj = 0,
		ModelFileType_Blender,
		ModelFileType_Gltf,
		ModelFileType_Stl,
		ModelFileType_Fbx
	};

	struct Mesh {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		Material* material{ nullptr };

		~Mesh() = default;
	};

	struct Model {
		std::string name{};
		std::vector<Mesh> meshes{};

		~Model() = default;
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

	class Pipeline : public ApplicationLayer {
	public:
		static inline Pipeline* get() { return m_instance; }

		static Shader load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		static Texture load_texture_into_memory(std::string_view name, std::string_view path, bool flip);
		static void destroy_shader_resource(Shader* shader);
		static void destroy_texture_resource(Texture* texture);

		Pipeline(Window* window_layer);
		virtual ~Pipeline() override;

		Window* get_window() { return m_window_layer; }
		void push_renderer(PipelineRenderer* renderer);
		PipelineRenderer* get_renderer(std::string_view name);
		void remove_renderer(std::string_view name);

		virtual void on_update() override;

	private:
		static Pipeline* m_instance;

		std::vector<PipelineRenderer*> m_renderers{};
		Window* m_window_layer{ nullptr };
	};

}

#endif