#ifndef __GHL_CORE_SCENE_MANAGER_HPP__
#define __GHL_CORE_SCENE_MANAGER_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"

#include <entt/entt.hpp>

namespace ghl {

	typedef void (*PtrFunSystem)(entt::registry&);

	class Scene {
	public:
		inline Scene(std::string_view name) : m_name(name) {}
		~Scene() = default;

		inline const std::string& get_name() const { return m_name; }
		inline entt::registry& get_registry() { return m_reg; }
		inline const entt::registry& get_registry() const { return m_reg; }

		void push_system(PtrFunSystem system);

	private:
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::string m_name{};
		entt::registry m_reg{};
		std::vector<PtrFunSystem> m_systems;
	};

	class SceneManager : public ApplicationLayer {
	public:
		static inline Scene* get_active() { return m_instance->m_active_scene; }
		static inline Scene* get(std::string_view name) { return m_instance->impl_get(name); }

		static inline Scene* push(std::string_view name) { return m_instance->impl_push(name); }
		static inline Scene* push(std::string_view name, std::string_view path) { return m_instance->impl_push(name, path); }
		static inline void push_system(PtrFunSystem system) { return m_instance->impl_push_system(system); }

		static inline void remove(std::string_view name) { m_instance->impl_remove(name); }

		static inline Scene* set_active(std::string_view name) { m_instance->impl_set_active(name); }
		static inline void set_active(Scene* scene) { m_instance->impl_set_active(scene); }

		SceneManager();
		virtual ~SceneManager() override;

		virtual void on_update() override;

	private:
		Scene* impl_get(std::string_view name);
		void impl_remove(std::string_view name);

		Scene* impl_set_active(std::string_view name);
		void impl_set_active(Scene* scene);

		Scene* impl_push(std::string_view name);
		Scene* impl_push(std::string_view name, std::string_view path);
		void impl_push_system(PtrFunSystem system);

		static SceneManager* m_instance;

		std::vector<Scene*> m_scenes{};
		std::vector<PtrFunSystem> m_systems{};
		Scene* m_active_scene{ nullptr };
	};

}

#endif
