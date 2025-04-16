#include <filesystem>
#include "y3.hpp"
#include "lua_bindings.hpp"

namespace fs = std::filesystem;
using namespace etna;

Window* y3::g_window = nullptr;
sol::state y3::lua;

y3::y3(uint32_t width, uint32_t height) {
	engine::init();

	g_window = new Window({
		.width = width,
		.height = height,
		.title = "y3 - Etna",
		.captureMouse = true,
	});

	m_renderer = new Renderer({});

	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

	y3_table = lua.create_named_table("y3");

	y3_table.set_function(
		"add_global_script",
		sol::overload(
			// from script
			[this](ScriptHandle script) { this->addGlobalScript(script); },

			// from file
			[this](const std::string& name, sol::table data) {
				this->addGlobalScript(create_script_from_file(name, data));
			}));

	y3_table.set_function("remove_global_script", [this](const std::string& name) {
		removeGlobalScript(name);
	});

	y3_table.set_function("create_script", sol::overload(
											   // from script
											   &create_script,
											   // from file
											   &create_script_from_file));

	y3_table.set_function("create_camera", &create_camera);

	y3_table.set_function("switch_scene", [this](const std::string& sceneName) {
		switchScene(sceneName);
	});

	y3_table.set_function("destroy_scene", [this](const std::string& sceneName) {
		destroyScene(sceneName);
	});

	y3_table.set_function("create_grid_material", create_grid_material);
	y3_table.set_function("create_color_material", engine::createColorMaterial);

	y3_table.set_function("create_mesh", create_mesh);
	y3_table.set_function("get_sphere", engine::getSphere);
	y3_table.set_function("get_cube", engine::getCube);
	y3_table.set_function("get_pyramid", engine::getPyramid);

	y3_table.set_function("is_key_down", [](int key) {
		return g_window->isKeyPressed(static_cast<Key>(key));
	});

	y3_table.set_function("key_clicked", [](int key) {
		return g_window->isKeyClicked(static_cast<Key>(key));
	});

	y3_table.set_function("mouse_x", []() { return g_window->getMouseX(); });
	y3_table.set_function("mouse_y", []() { return g_window->getMouseY(); });
	y3_table.set_function("mouse_dx", []() { return g_window->mouseDeltaX(); });
	y3_table.set_function("mouse_dy", []() { return g_window->mouseDeltaY(); });

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

	lua.collect_garbage();
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

	sol::protected_function_result result = lua.script_file(sceneName + ".lua");

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
