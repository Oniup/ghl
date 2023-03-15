#ifndef __GHL_CORE_SCENE_MANAGER_HPP__
#define __GHL_CORE_SCENE_MANAGER_HPP__

#include "ghl/utils/utils.hpp"
#include "ghl/core/application_layer.hpp"

namespace ghl {

	class System {
	public:
		// TODO(Ewan): implement on_fixed_update
		virtual void on_fixed_update(entt::registry& registry) {}
		virtual void on_update(entt::registry& registry) {}
		virtual void on_late_update(entt::registry& registry) {}
		virtual void on_before_render(entt::registry& registry) {}
	};

	class Scene {
	public:
		inline Scene(std::string_view name) : m_name(name) {}
		~Scene() = default;

		inline const std::string& get_name() const { return m_name; }
		inline entt::registry& get_registry() { return m_reg; }
		inline const entt::registry& get_registry() const { return m_reg; }

		template <typename _System, typename ... _Args>
		void push_system(_Args ... args) {
			m_systems.push_back(new _System{ args... });
		}

	private:
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		std::string m_name{};
		entt::registry m_reg{};
		std::vector<System*> m_systems;
	};

	class SceneManager : public ApplicationLayer {
	public:
		static SceneManager* get() { return m_instance; }

		SceneManager();
		virtual ~SceneManager() override;

		Scene* get_scene(std::string_view name);
		inline Scene* get_active_scene() { return m_active_scene; }

		Scene* set_active(std::string_view name);
		void set_active(Scene* scene);

		void remove(std::string_view name);

		Scene* push(std::string_view name);
		Scene* push(std::string_view name, std::string_view path);

		template <typename _System, typename ... _Args>
		void push_system(_Args ... args) {
			m_systems.push_back(new _System(std::forward<_Args...>(args...)));
		}

		virtual void on_update() override;

	private:
		static SceneManager* m_instance;

		std::vector<Scene*> m_scenes{};
		std::vector<System*> m_systems{};
		Scene* m_active_scene{ nullptr };
	};

}

#endif
