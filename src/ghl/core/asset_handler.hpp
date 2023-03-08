#ifndef __GHL_CORE_ASSET_HANDLER_HPP__
#define __GHL_CORE_ASSET_HANDLER_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"
#include "ghl/renderer/pipeline.hpp"

namespace ghl {

	class AssetHandler : public ApplicationLayer {
	public:
		static inline AssetHandler* get() { return m_instance; }

		AssetHandler();
		~AssetHandler();

		Shader* load_shader_into_memory(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		Texture* load_texture_into_memory(std::string_view name, std::string_view path, bool flip = true);
		Model* load_model_into_memory(std::string_view name, std::string_view path);

		Shader* get_shader(std::string_view name);
		Texture* get_texture(std::string_view name);
		Model* get_model(std::string_view name);

		void remove_shader(std::string_view name);
		void remove_texture(std::string_view name);
		void remove_model(std::string_view name);

	private:
		static AssetHandler* m_instance;

		std::vector<Shader> m_shaders{};
		std::vector<Texture> m_textures{};
		std::vector<Model> m_models{};
	};

}

#endif