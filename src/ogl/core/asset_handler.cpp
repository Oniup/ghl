#include "ogl/core/asset_handler.hpp"
#include "ogl/core/debug.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ogl {

	AssetHandler* AssetHandler::m_instance = nullptr;

	AssetHandler::AssetHandler() : ApplicationLayer(OGL_CORE_ASSET_HANDLER_NAME) {
		OGL_ASSERT(m_instance != nullptr, "AssetHandler::AssetHandler() -> cannot created multiple asset handler application layers");

		m_instance = this;

		// TODO(Ewan): change to read in a already compiled shader for the defaults
		Shader* shader = load_shader_into_memory(OGL_ASSET_MATERIAL_PHONG_DEFAULT_NAME, OGL_ASSET_SHADER_PHONG_DEFAULT_VERT_PATH, OGL_ASSET_SHADER_PHONG_DEFAULT_FRAG_PATH);
		Material* material = load_material_into_memory(OGL_ASSET_MATERIAL_PHONG_DEFAULT_NAME);
		material->shader = shader;
	}

	AssetHandler::~AssetHandler() {
		for (Shader& shader : m_shaders) {
			Pipeline::destroy_shader_resource(&shader);
		}

		for (Texture& texture : m_textures) {
			Pipeline::destroy_texture_resource(&texture);
		}

		for (Model* model : m_static_models) {
			delete model;
		}
	}

	Shader* AssetHandler::load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path) {
		m_shaders.push_back(Pipeline::load_shader_into_memory(name, vertex_path, fragment_path));
		return &m_shaders.back();
	}

	Texture* AssetHandler::load_texture_into_memory(std::string_view name, std::string_view path, bool flip) {
		m_textures.push_back(Pipeline::load_texture_into_memory(name, path, flip));
		return &m_textures.back();
	}

	Material* AssetHandler::load_material_into_memory(std::string_view name) {
		m_materials.push_back({ name.data() });
		return &m_materials.back();
	}

	Model* AssetHandler::load_static_model_into_memory(std::string_view name, std::string_view path, ModelFileType file_type) {
		m_static_models.resize(m_static_models.size() + 1);
		m_static_models.back()->name = name;

		_load_mesh_data(m_static_models.back(), path, false, file_type);
		return m_static_models.back();
	}

	Model* AssetHandler::load_model_into_memory(std::string_view path, ModelFileType file_type) {
		Model* model = new Model();

		_load_mesh_data(model, path, true, file_type);
		return model;
	}

	Shader* AssetHandler::get_shader(std::string_view name) {
		for (Shader& shader : m_shaders) {
			if (shader.name == name) {
				return &shader;
			}
		}

		return nullptr;
	}

	Texture* AssetHandler::get_texture(std::string_view name) {
		for (Texture& texture : m_textures) {
			if (texture.name == name) {
				return &texture;
			}
		}

		return nullptr;
	}

	Material* AssetHandler::get_material(std::string_view name) {
		for (Material& material : m_materials) {
			if (material.name == name) {
				return &material;
			}
		}

		return nullptr;
	}

	Model* AssetHandler::get_static_model(std::string_view name) {
		return nullptr;
	}

	void AssetHandler::remove_shader(std::string_view name) {
		for (std::vector<Shader>::iterator shader = m_shaders.begin(); shader != m_shaders.end(); shader++) {
			if (shader->name == name) {
				Pipeline::destroy_shader_resource(&*shader);
				m_shaders.erase(shader);
				return;
			}
		}
	}

	void AssetHandler::remove_texture(std::string_view name) {
		for (std::vector<Texture>::iterator texture = m_textures.begin(); texture != m_textures.end(); texture++) {
			if (texture->name == name) {
				Pipeline::destroy_texture_resource(&*texture);
				m_textures.erase(texture);
				return;
			}
		}
	}

	void AssetHandler::remove_material(std::string_view name) {
		for (std::vector<Material>::iterator material = m_materials.begin(); material != m_materials.end(); material++) {
			if (material->name == name) {
				m_materials.erase(material);
				return;
			}
		}
	}

	void AssetHandler::remove_static_model(std::string_view name) {
		for (size_t i = 0; i < m_static_models.size(); i++) {
			if (m_static_models[i]->name == name) {
				delete m_static_models[i];
				m_static_models.erase(m_static_models.begin() + i);
				return;
			}
		}
	}

	void AssetHandler::remove_all_shaders() {
		m_shaders.clear();
	}

	void AssetHandler::remove_all_textures() {
		m_textures.clear();
	}

	void AssetHandler::remove_all_static_models() {
		for (Model* model : m_static_models) {
			delete model;
		}
		m_static_models.clear();
	}

	void AssetHandler::_load_mesh_data(Model* model, std::string_view path, bool is_dynamic, ModelFileType file_type, LoadModelProcessStep process_steps) {
		model->name = std::string(path.data() + std::string(path).find_last_of('/') + 1);
		std::string full_path = path.data() + std::string("/") + model->name + _get_model_file_type_as_str(file_type);

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(full_path, process_steps);
		if (scene == nullptr) {
			Debug::log("AssetHandler::_load_mesh_data(Model*, std::string_view, bool, ModelFileType) -> failed to load model path \"" + full_path + "\"", DebugType_Error);
			return;
		}

		_process_assimp_node(model, scene->mRootNode, scene);
	}

	void AssetHandler::_set_mesh_vertex_data(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Material* material, Mesh* mesh, bool is_dynamic) {
		mesh->vertices = std::move(vertices);
		mesh->indices = std::move(indices);
		mesh->material = material;
	}

	void AssetHandler::_process_assimp_node(Model* model, aiNode* node, const aiScene* scene) {
		for (uint32_t i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			model->meshes.emplace_back(_process_assimp_mesh(mesh, scene));
		}
		for (uint32_t i = 0; i < node->mNumChildren; i++) {
			_process_assimp_node(model, node->mChildren[i], scene);
		}
	}

	Mesh AssetHandler::_process_assimp_mesh(aiMesh* mesh, const aiScene* scene) {
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		// TODO(Ewan): load material

		for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;
			vertex.position = glm::vec3(
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			);
			vertex.normal = glm::vec3(
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			);
			if (mesh->mTextureCoords[0]) {
				vertex.uv = glm::vec2(
					mesh->mTextureCoords[0][i].x,
					mesh->mTextureCoords[0][i].y
				);
			}

			vertices.push_back(vertex);
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		return Mesh{ vertices, indices};
	}

	std::string AssetHandler::_get_model_file_type_as_str(ModelFileType file_type) const {
		std::string str{};

		switch (file_type) {
		case ModelFileType_Obj:
			 str = ".obj";
			break;
		case ModelFileType_Blender:
			 str = ".blend";
			break;
		case ModelFileType_Gltf:
			 str = ".gltf";
			break;
		case ModelFileType_Stl:
			 str = ".stl";
			break;
		case ModelFileType_Fbx:
			 str = ".fbx";
			break;
		}

		return str;
	}

	NonRunTimeAssetHandler::NonRunTimeAssetHandler() : ApplicationLayer(OGL_CORE_NON_RUNTIME_ASSET_HANDLER_NAME) {

	}

}