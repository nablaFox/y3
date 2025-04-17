#include <filesystem>
#include "y3.hpp"

namespace fs = std::filesystem;
using namespace etna;

Window* y3::g_window = nullptr;

y3::y3(uint32_t width, uint32_t height) {
	engine::init();

	g_window = new Window({
		.width = width,
		.height = height,
		.title = "y3 - Etna",
		.captureMouse = true,
	});

	m_renderer = new Renderer({});

	m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

	y3_table = m_lua.create_named_table("y3");

	initLuaBindings();

	initLuaTypes();
}

y3::~y3() {
	delete g_window;
	delete m_renderer;
	m_currScene = nullptr;

	for (auto& [_, script] : m_globalScripts) {
		if (script->m_info.onDestroy != nullptr) {
			script->m_info.onDestroy(nullptr, script->m_info.data, nullptr);
		}
	}

	m_lua.collect_garbage();
}

void y3::run() {
	while (!g_window->shouldClose()) {
		engine::updateTime();

		g_window->pollEvents();

		m_currScene->applyUpdateScripts();

		for (auto& [_, script] : m_globalScripts) {
			if (script->m_info.onUpdate != nullptr) {
				script->m_info.onUpdate(nullptr, script->m_info.data,
										engine::getDeltaTime(), m_currScene);
			}
		}

		m_currScene->render(*m_renderer);

		g_window->swapBuffers();
	}
}

void y3::switchScene(const std::string& sceneName) {
	auto it = m_scenes.find(sceneName);

	if (it != m_scenes.end()) {
		m_currScene->applySleepScripts();
		m_currScene = it->second.get();
		return;
	}

	std::string path = fs::current_path().string() + "/" + sceneName + ".lua";

	if (!fs::exists(path)) {
		throw std::runtime_error("Scene not found in path : " + path);
	}

	sol::protected_function_result result = m_lua.script_file(sceneName + ".lua");

	if (!result.valid()) {
		throw std::runtime_error("Failed to load scene: " + sceneName);
	}

	auto scene = std::make_unique<Scene>();

	sol::table sceneTable = result;

	for (const auto& pair : sceneTable) {
		SceneNode node = pair.second.as<SceneNode>();

		sol::object obj = pair.second;

		if (obj.is<SceneNode>()) {
			SceneNode node = obj.as<SceneNode>();
			scene->addNode(node);
		}
	}

	if (m_currScene != nullptr) {
		m_currScene->applySleepScripts();
	}

	scene->applyStartScripts();

	m_currScene = scene.get();
	m_scenes[sceneName] = std::move(scene);
}

void y3::destroyScene(const std::string& sceneName) {
	auto it = m_scenes.find(sceneName);

	if (it != m_scenes.end() && it->first != "main") {
		m_scenes.erase(it);
	}
}

void y3::addGlobalScript(ScriptHandle script) {
	m_globalScripts[script->m_info.name] = script;

	if (script->m_info.onStart != nullptr) {
		script->m_info.onStart(nullptr, script->m_info.data, nullptr);
	}
}

void y3::removeGlobalScript(const std::string& name) {
	auto it = m_globalScripts.find(name);

	if (it != m_globalScripts.end()) {
		it->second->m_info.onDestroy(nullptr, it->second->m_info.data, nullptr);
		m_globalScripts.erase(it);
	}
}
