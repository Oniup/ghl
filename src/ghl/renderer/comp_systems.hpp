#ifndef __GHL_RENDERER_COMPONENTS_HPP__
#define __GHL_RENDERER_COMPONENTS_HPP__

#include "ghl/utils/utils.hpp"

#include <entt/entt.hpp>

namespace ghl {

	struct TransformComponent {
		glm::vec3 position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 scale{ 1.0f, 1.0f, 1.0f };
		glm::vec4 rotation{ 0.0f, 0.0f, 1.0f, 0.0f };
	};

	struct SpriteRendererComponent {
		// TODO: implement renderer ...
	};

	struct SpriteAnimationComponent {
		SpriteRendererComponent* sprite_renderer{ nullptr };
		// TODO: implement basic sprite renderer before this
	};

	struct RenderSystems {
		void sprite_renderer_system(entt::registry& registry);
		void sprite_animation_system(entt::registry& registry);
	};

}

#endif