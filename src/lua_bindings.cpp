#include "y3.hpp"

using namespace etna;

static Transform getTransform(sol::table params) {
	Vec3 defaultPosition{0, 0, 0};

	const Transform transform{
		.position = params.get_or("position", defaultPosition),
		.yaw = params.get_or("yaw", 0.0f),
		.pitch = params.get_or("pitch", 0.0f),
		.roll = params.get_or("roll", 0.0f),
	};

	return transform;
}

static std::vector<ScriptHandle> getScripts(sol::table params) {
	if (params["scripts"].is<ScriptHandle>()) {
		return {params["scripts"]};
	}

	if (params["scripts"].is<sol::table>()) {
		std::vector<ScriptHandle> scripts;

		sol::table scriptTable = params["scripts"];

		for (auto& kv : scriptTable) {
			sol::object value = kv.second;
			if (value.is<ScriptHandle>()) {
				scripts.push_back(value.as<ScriptHandle>());
			}
		}

		return scripts;
	}

	return {};
}

static void loadScriptFunctions(sol::table scriptTable,
								Script::HookFunc&& onCreate,
								Script::UpdateFunc&& onUpdate,
								Script::HookFunc&& onSleep,
								Script::HookFunc&& onDestroy) {
	sol::function update = scriptTable["update"];
	sol::function start = scriptTable["start"];
	sol::function sleep = scriptTable["sleep"];
	sol::function destroy = scriptTable["destroy"];

	if (update.valid()) {
		onUpdate = [update](_SceneNode* node, sol::table data, float dt,
							Scene* scene) {
			sol::protected_function_result result = update(node, data, dt, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in update: " << err.what() << std::endl;
			}
		};
	}

	if (start.valid()) {
		onCreate = [start](_SceneNode* node, sol::table data, Scene* scene) {
			sol::protected_function_result result = start(node, data, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in start: " << err.what() << std::endl;
			}
		};
	}

	if (sleep.valid()) {
		onSleep = [sleep](_SceneNode* node, sol::table data, Scene* scene) {
			sol::protected_function_result result = sleep(node, data, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in sleep: " << err.what() << std::endl;
			}
		};
	}

	if (destroy.valid()) {
		onDestroy = [destroy](_SceneNode* node, sol::table data, Scene* scene) {
			sol::protected_function_result result = destroy(node, data, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in destroy: " << err.what() << std::endl;
			}
		};
	}
}

ScriptHandle create_script(sol::table scriptTable) {
	sol::function update = scriptTable["update"];
	sol::function start = scriptTable["start"];
	sol::function sleep = scriptTable["sleep"];
	sol::function destroy = scriptTable["destroy"];
	sol::table data = scriptTable["data"];

	std::function<void(_SceneNode*, sol::table, Scene* const)> onCreate;

	std::function<void(_SceneNode*, sol::table, float, Scene* const)> onUpdate;

	std::function<void(_SceneNode*, sol::table, Scene* const)> onSleep;

	std::function<void(_SceneNode*, sol::table, Scene* const)> onDestroy;

	loadScriptFunctions(scriptTable, std::move(onCreate), std::move(onUpdate),
						std::move(onSleep), std::move(onDestroy));

	const Script::CreateInfo info{
		.name = scriptTable["name"],
		.onUpdate = onUpdate,
		.onStart = onCreate,
		.onSleep = onSleep,
		.onDestroy = onDestroy,
		.data = data,

	};

	return std::make_shared<Script>(info);
}

SceneNode create_mesh(sol::table params) {
	scene::MeshNodeCreateInfo info{
		.name = params["name"],
		.mesh = params["mesh"],
		.material = params["material"],
		.transform = getTransform(params),
		.scripts = getScripts(params),
	};

	return scene::createMeshNode(info);
}

SceneNode create_camera(sol::table params) {
	Camera::CreateInfo defaultCameraInfo{};
	Viewport defaultViewport{};

	scene::CameraNodeCreateInfo info{
		.name = params["name"],
		.cameraInfo = params["cameraInfo"].get_or(defaultCameraInfo),
		.viewport = params["viewport"].get_or(defaultViewport),
		.renderTarget = y3::g_window,
		.transform = getTransform(params),
		.scripts = getScripts(params),
	};

	return scene::createCameraNode(info);
}

MaterialHandle create_grid_material(sol::table params) {
	engine::GridMaterialParams gridParams{
		.color = params["color"],
		.gridColor = params["gridColor"],
		.gridSpacing = params["gridSpacing"],
		.thickness = params["thickness"],
	};

	return engine::createGridMaterial(gridParams);
}

void y3::initLuaBindings() {
	y3_table.set_function("add_global_script",
						  sol::overload([this](ScriptHandle script) {
							  this->addGlobalScript(script);
						  }));

	y3_table.set_function("remove_global_script", [this](const std::string& name) {
		removeGlobalScript(name);
	});

	y3_table.set_function("create_script", sol::overload(&create_script));

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
}
