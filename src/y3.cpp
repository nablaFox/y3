#include <filesystem>
#include "y3.hpp"
#include "lua_bindings.hpp"

namespace fs = std::filesystem;
using namespace etna;

Window* y3::g_window = nullptr;

y3::y3(uint32_t width, uint32_t height) {
	etna::engine::init();

	g_window = new Window({
		.width = width,
		.height = height,
		.title = "y3 - Etna",
		.captureMouse = true,
	});

	m_renderer = new Renderer({});

	lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::package);

	y3_table = lua.create_named_table("y3");

	y3_table.set_function("create_camera", create_camera);
	y3_table.set_function("create_mesh", create_mesh);
	y3_table.set_function("create_script", create_script);
	y3_table.set_function("create_grid_material", create_grid_material);
	y3_table.set_function("get_sphere", engine::getSphere);

	initLuaTypes();
}

y3::~y3() {
	delete g_window;
	delete m_renderer;

	for (auto& [_, scene] : m_scenes) {
		delete scene;
	}
}

void y3::run() {
	while (!g_window->shouldClose()) {
		g_window->pollEvents();

		m_currScene->updateNodes();

		m_currScene->render(*m_renderer);

		g_window->swapBuffers();
	}
}

void y3::switchScene(const std::string& sceneName) {
	auto it = m_scenes.find(sceneName);

	if (it != m_scenes.end()) {
		m_currScene = it->second;
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

	Scene* scene = new Scene();

	sol::table sceneTable = result;

	for (const auto& pair : sceneTable) {
		SceneNode node = pair.second.as<SceneNode>();

		sol::object obj = pair.second;

		if (obj.is<SceneNode>()) {
			SceneNode node = obj.as<SceneNode>();
			scene->addNode(node);
		}
	}

	m_currScene = scene;
	m_currScene->setupNodes();
}
