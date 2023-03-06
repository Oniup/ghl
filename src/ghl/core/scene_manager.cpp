#include "ghl/core/scene_manager.hpp"
#include "ghl/core/debug.hpp"

namespace ghl {

	SceneManager* SceneManager::m_instance = nullptr;

	void Scene::push_system(PtrFunSystem system) {
		if (system != nullptr) {
			m_systems.push_back(system);
		}
		else {
			Debug::log("Scene::push_system(PtrFunSystem) -> system is null");
		}
	}

	SceneManager::SceneManager() : ApplicationLayer(GHL_SCENE_MANAGER_LAYER_NAME) {
		GHL_ASSERT(m_instance != nullptr, "SceneManager::SceneManager() -> cannot have multiple scene manager layers");

		m_instance = this;
	}

	SceneManager::~SceneManager() {
		for (Scene* scene : m_scenes) {
			delete scene;
		}
	}

	Scene* SceneManager::impl_get(std::string_view name) {
		for (Scene* scene : m_scenes) {
			if (scene->get_name() == name) {
				return scene;
			}
		}
		return nullptr;
	}

	Scene* SceneManager::impl_push(std::string_view name) {
		m_scenes.push_back(new Scene(name));
		return m_scenes.back();
	}

	Scene* SceneManager::impl_push(std::string_view name, std::string_view path) {
		// TODO: implement serialization scene so then this file can read the serialized scene
		return impl_push(name);
	}

	void SceneManager::impl_push_system(PtrFunSystem system) {
		if (system != nullptr) {
			m_systems.push_back(system);
		}
		else {
			Debug::log("SceneManager::push_system(PtrFunSystem) -> system is null");
		}
	}

	void SceneManager::impl_remove(std::string_view name) {
		for (size_t i = 0; i < m_scenes.size(); i++) {
			if (m_scenes[i]->get_name() == name) {
				m_scenes.erase(m_scenes.begin() + i);
				return;
			}
		}

		Debug::log("could not find scene named \"" + std::string(name) + "\"");
	}

	Scene* SceneManager::impl_set_active(std::string_view name) {
		for (Scene* scene : m_scenes) {
			if (scene->get_name() == name) {
				m_active_scene = scene;
				return m_active_scene;
			}
		}

		Debug::log("the scene \"" + std::string(name) + "\" doesn't exist or hasn't been loaded into memory yet");
		return nullptr;
	}

	void SceneManager::impl_set_active(Scene* scene) {
		GHL_ASSERT(scene == nullptr, "SceneManager::set_active(Scene*) -> scene is null");

		m_active_scene = scene;
	}

	void SceneManager::on_update() {
		for (PtrFunSystem system : m_systems) {
			system(m_active_scene->get_registry());
		}
	}

}