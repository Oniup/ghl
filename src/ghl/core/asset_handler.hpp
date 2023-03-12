#ifndef __GHL_CORE_ASSET_HANDLER_HPP__
#define __GHL_CORE_ASSET_HANDLER_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"
#include "ghl/renderer/pipeline.hpp"

struct aiMesh;
struct aiScene;
struct aiNode;

namespace ghl {

	class AssetHandler : public ApplicationLayer {
	public:
		static inline AssetHandler* get() { return m_instance; }

		AssetHandler();
		~AssetHandler();

		Shader* load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		Texture* load_texture_into_memory(std::string_view name, std::string_view path, bool flip = true);
		Model* load_static_model_into_memory(std::string_view name, std::string_view path, ModelFileType file_type);
		Model* load_model_into_memory(std::string_view path, ModelFileType file_type);

		Shader* get_shader(std::string_view name);
		Texture* get_texture(std::string_view name);
		Model* get_static_model(std::string_view name);

		void remove_shader(std::string_view name);
		void remove_texture(std::string_view name);
		void remove_static_model(std::string_view name);

		void remove_all_shaders();
		void remove_all_textures();
		void remove_all_static_models();

	private:
		void _load_mesh_data(Model* model, std::string_view path, bool is_dynamic, ModelFileType file_type);
		void _set_mesh_vertex_data(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Material* material, Mesh* mesh, bool is_dynamic);
		void _process_assimp_node(Model* model, aiNode* node, const aiScene* scene);
		Mesh _process_assimp_mesh(aiMesh* mesh, const aiScene* scene);
		std::string _get_model_file_type_as_str(ModelFileType file_type) const;

		static AssetHandler* m_instance;

		std::vector<Shader> m_shaders{};
		std::vector<Texture> m_textures{};
		std::vector<Model*> m_static_models{};
	};

}

#endif