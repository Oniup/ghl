#ifndef __GHL_RENDERER_PIPELINE_HPP__
#define __GHL_RENDERER_PIPELINE_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"

namespace ghl {

	struct Shader {
		std::string name;
		uint32_t id;
	};

	struct Texture {
		std::string name;
		glm::ivec2 size;
		uint32_t id;
	};

	class Pipeline : public ApplicationLayer {
	public:
		static inline Pipeline* get() { return m_instance; }

		Pipeline();
		virtual ~Pipeline() override;

		virtual void on_update() override;

		Shader* get_shader(std::string_view name);
		Texture* get_texture(std::string_view name);

		Shader* load_shader(std::string_view name, std::string_view vertex_path, std::string_view fragment_path);
		Texture* load_texture(std::string_view name, std::string_view path, bool flip = true);

		void remove_shader(std::string_view name);
		void remove_texture(std::string_view name);

	private:
		static Pipeline* m_instance;

		std::vector<Shader> m_shaders;
		std::vector<Texture> m_textures;
	};

}

#endif