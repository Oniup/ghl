#ifndef __OGL_CORE_ASSET_HANDLER_HPP__
#define __OGL_CORE_ASSET_HANDLER_HPP__

#include "ogl/utils/utils.hpp"
#include "ogl/core/application_layer.hpp"
#include "ogl/renderer/pipeline.hpp"

struct aiMesh;
struct aiScene;
struct aiNode;

#define OGL_ASSET_MATERIAL_PHONG_DEFAULT_NAME		"phong default"
#define OGL_ASSET_SHADER_PHONG_DEFAULT_FRAG_PATH	"ogl/assets/shaders/phong_default.frag"
#define OGL_ASSET_SHADER_PHONG_DEFAULT_VERT_PATH	"ogl/assets/shaders/phong_default.vert"

#define OGL_ASSET_SHADER_PBR_DEFAULT_NAME			"pbr default"

namespace ogl {

	typedef int LoadModelProcessStep;

	enum LoadModelProcessStep_ {
		LoadModelProcessStep_CalcTangentSpace = 0x1,
		LoadModelProcessStep_JoinIdenticalVertices = 0x2,
		LoadModelProcessStep_Triangulate = 0x8,
		LoadModelProcessStep_RemoveComponent = 0x10,
		LoadModelProcessStep_GenNormals = 0x20,
		LoadModelProcessStep_GenSmoothNormals = 0x40,
		LoadModelProcessStep_SplitLargeMeshes = 0x80,
		LoadModelProcessStep_PreTransformVertices = 0x100,
		LoadModelProcessStep_LimitBoneWeights = 0x200,
		LoadModelProcessStep_ValidateDataStructure = 0x400,
		LoadModelProcessStep_ImproveCacheLocality = 0x800,
		LoadModelProcessStep_RemoveRedundantMaterials = 0x1000,
		LoadModelProcessStep_FixInfacingNormals = 0x2000,
		LoadModelProcessStep_PopulateArmatureData = 0x4000,
		LoadModelProcessStep_SortByPType = 0x8000,
		LoadModelProcessStep_FindDegenerates = 0x10000,
		LoadModelProcessStep_FindInvalidData = 0x20000,
		LoadModelProcessStep_GenUVCoords = 0x40000,
		LoadModelProcessStep_TransformUVCoords = 0x80000,
		LoadModelProcessStep_FindInstances = 0x100000,
		LoadModelProcessStep_OptimizeMeshes  = 0x200000,
		LoadModelProcessStep_OptimizeGraph  = 0x400000,
		LoadModelProcessStep_FlipUVs = 0x800000,
		LoadModelProcessStep_FlipWindingOrder  = 0x1000000,
		LoadModelProcessStep_SplitByBoneCount  = 0x2000000,
		LoadModelProcessStep_Debone  = 0x4000000,
		LoadModelProcessStep_GlobalScale = 0x8000000,
		LoadModelProcessStep_EmbedTextures  = 0x10000000,
		LoadModelProcessStep_ForceGenNormals = 0x20000000,
		LoadModelProcessStep_DropNormals = 0x40000000,
		LoadModelProcessStep_GenBoundingBoxes = 0x80000000
	};

	class AssetHandler : public ApplicationLayer {
	public:
		static inline AssetHandler* get() { return m_instance; }

		AssetHandler();
		~AssetHandler();

		Shader* load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		Texture* load_texture_into_memory(std::string_view name, std::string_view path, bool flip = true);
		Material* load_material_into_memory(std::string_view name);
		Model* load_static_model_into_memory(std::string_view name, std::string_view path, ModelFileType file_type);
		Model* load_model_into_memory(std::string_view path, ModelFileType file_type);

		Shader* get_shader(std::string_view name);
		Texture* get_texture(std::string_view name);
		Material* get_material(std::string_view name);
		Model* get_static_model(std::string_view name);

		void remove_shader(std::string_view name);
		void remove_texture(std::string_view name);
		void remove_material(std::string_view name);
		void remove_static_model(std::string_view name);

		void remove_all_shaders();
		void remove_all_textures();
		void remove_all_static_models();

	private:
		void _load_mesh_data(Model* model, std::string_view path, bool is_dynamic, ModelFileType file_type, LoadModelProcessStep = LoadModelProcessStep_Triangulate | LoadModelProcessStep_FlipUVs);
		void _set_mesh_vertex_data(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Material* material, Mesh* mesh, bool is_dynamic);
		void _process_assimp_node(Model* model, aiNode* node, const aiScene* scene);
		Mesh _process_assimp_mesh(aiMesh* mesh, const aiScene* scene);
		std::string _get_model_file_type_as_str(ModelFileType file_type) const;

		static AssetHandler* m_instance;

		std::vector<Shader> m_shaders{};
		std::vector<Texture> m_textures{};
		std::vector<Material> m_materials{};
		std::vector<Model*> m_static_models{};
	};

	// TODO(Ewan): implement serialization eventually
	class NonRunTimeAssetHandler : public ApplicationLayer {
	public:
		NonRunTimeAssetHandler();
		virtual ~NonRunTimeAssetHandler() = default;
	};

}

#endif