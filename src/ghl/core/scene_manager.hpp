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
		static SceneManager* get() { return m_instance; }

		SceneManager();
		virtual ~SceneManager() override;

		Scene* get(std::string_view name);
		void remove(std::string_view name);

		Scene* set_active(std::string_view name);
		void set_active(Scene* scene);

		Scene* push(std::string_view name);
		Scene* push(std::string_view name, std::string_view path);
		void push_system(PtrFunSystem system);

		virtual void on_update() override;

	private:
		static SceneManager* m_instance;

		std::vector<Scene*> m_scenes{};
		std::vector<PtrFunSystem> m_systems{};
		Scene* m_active_scene{ nullptr };
	};

}

#endif
