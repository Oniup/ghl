#include "ogl/renderer/renderer_comps.hpp"

namespace ogl {

	ModelRendererComponent::~ModelRendererComponent() {
		if (!is_static && model != nullptr) {
			delete model;
		}
	}

	void RenderSystems::sprite_renderer_system(entt::registry& registry) {
		// TODO: ...
	}

	void RenderSystems::sprite_atlas_animation_system(entt::registry& registry) {
		// TODO: ...
	}

}
