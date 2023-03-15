#ifndef __GHL_RENDERER_RENDERER_HPP__
#define __GHL_RENDERER_RENDERER_HPP__

#include "ghl/utils/utils.hpp" 
#include "ghl/renderer/pipeline.hpp"

namespace ghl {

	class BasicRenderer : public PipelineRenderer {
	public:
		BasicRenderer();
		virtual ~BasicRenderer() override = default;

		virtual void on_render() override;
		void submit(Mesh* mesh, const glm::mat4& model_matrix);

	private:
		struct _RenderPass {
			Material* material{ nullptr };
			Mesh* mesh{ nullptr };
			glm::mat4 transforms{};
		};

		class _CollectData : public System {
		public:
			_CollectData(BasicRenderer* renderer);
			virtual void on_before_render(entt::registry& registry) override;

		private:
			BasicRenderer* m_renderer{ nullptr };
		};

		std::vector<_RenderPass> m_render_passes{};
		VertexArrayBuffer m_vertex_array{};
	};

}

#endif