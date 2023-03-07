#include "ghl/core/scene_manager.hpp"
#include "ghl/core/debug.hpp"

namespace ghl {

	SceneManager* SceneManager::m_instance = nullptr;

	void Scene::push_system(PtrFunSystem system) {
		if (system != nullptr) {
			m_systems.push_back(system);
		}
		else {
			Debug::log("Scene::push_system(PtrFunSystem) -> ECS system is null therefore will not at this", DebugType_Warning);
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

	Scene* SceneManager::get(std::string_view name) {
		for (Scene* scene : m_scenes) {
			if (scene->get_name() == name) {
				return scene;
			}
		}
		return nullptr;
	}

	Scene* SceneManager::push(std::string_view name) {
		m_scenes.push_back(new Scene(name));
		return m_scenes.back();
	}

	Scene* SceneManager::push(std::string_view name, std::string_view path) {
		// TODO: implement serialization scene so then this file can read the serialized scene
		return push(name);
	}

	void SceneManager::push_system(PtrFunSystem system) {
		if (system != nullptr) {
			m_systems.push_back(system);
		}
		else {
			Debug::log("SceneManager::push_system(PtrFunSystem) -> system is null", DebugType_Warning);
		}
	}

	void SceneManager::remove(std::string_view name) {
		for (size_t i = 0; i < m_scenes.size(); i++) {
			if (m_scenes[i]->get_name() == name) {
				m_scenes.erase(m_scenes.begin() + i);
				return;
			}
		}

		Debug::log("could not find scene named \"" + std::string(name) + "\"", DebugType_Error);
	}

	Scene* SceneManager::set_active(std::string_view name) {
		for (Scene* scene : m_scenes) {
			if (scene->get_name() == name) {
				m_active_scene = scene;
				return m_active_scene;
			}
		}

		Debug::log("the scene \"" + std::string(name) + "\" doesn't exist or hasn't been loaded into memory yet", DebugType_Error);
		return nullptr;
	}

	void SceneManager::set_active(Scene* scene) {
		GHL_ASSERT(scene == nullptr, "SceneManager::set_active(Scene*) -> scene is null");

		m_active_scene = scene;
	}

	void SceneManager::on_update() {
		for (PtrFunSystem system : m_systems) {
			system(m_active_scene->get_registry());
		}
	}

}