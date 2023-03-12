#ifndef __GHL_RENDERER_COMPONENTS_HPP__
#define __GHL_RENDERER_COMPONENTS_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/renderer/pipeline.hpp"

#include <entt/entt.hpp>

namespace ghl {

	struct TransformComponent {
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec4 rotation{ 0.0f, 0.0f, 1.0f, 0.0f };
	};

	struct SpriteRendererComponent {
		ghl::Shader* shader{ nullptr };
		ghl::Texture* texture{ nullptr };
		glm::vec2 uv{};
	};

	struct SpriteAtlasAnimatorComponent {
		SpriteRendererComponent* sprite_renderer{ nullptr };
		uint32_t current_frame{};
		std::vector<float> frame_lengths{};
		glm::vec2 frame_size{};
		glm::ivec2 frame_count{};
		glm::vec2 frame_uv_size{};
	};

	struct ModelRendererComponent {
		~ModelRendererComponent();

		bool is_static{ false };
		Model* model{ nullptr };
		// TODO(Ewan): rework how model data is retrieved 
		// TODO(Ewan): implement model animations
	};

	// struct ModelAnimatorComponent {

	// };

	struct RenderSystems {
		void sprite_renderer_system(entt::registry& registry);
		void sprite_atlas_animation_system(entt::registry& registry);
	};

}

#endif