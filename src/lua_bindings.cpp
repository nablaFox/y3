#include "y3.hpp"
#include "lua_bindings.hpp"

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

ScriptHandle create_script_from_file(std::string name, sol::table data) {
	std::string path = "scripts/" + name + ".lua";

	sol::load_result scriptFile = y3::lua.load_file(path);

	if (!scriptFile.valid()) {
		sol::error err = scriptFile;
		std::cerr << "Failed to load script \"" << path << "\": " << err.what()
				  << std::endl;
		return nullptr;
	}

	sol::protected_function scriptFunc = scriptFile;
	sol::protected_function_result result = scriptFunc();
	if (!result.valid()) {
		sol::error err = result;
		std::cerr << "Error executing script \"" << path << "\": " << err.what()
				  << std::endl;
		return nullptr;
	}

	sol::object scriptReturn = result;
	if (scriptReturn.get_type() != sol::type::table) {
		std::cerr << "Script \"" << path << "\" did not return a table."
				  << std::endl;
		return nullptr;
	}

	sol::table scriptTable = scriptReturn.as<sol::table>();

	std::function<void(_SceneNode*, sol::table, float, Scene* const)> onUpdate;

	std::function<void(_SceneNode*, sol::table, Scene* const)> onCreate;

	std::function<void(_SceneNode*, sol::table, Scene* const)> onSleep;

	std::function<void(_SceneNode*, sol::table, Scene* const)> onDestroy;

	loadScriptFunctions(scriptTable, std::move(onCreate), std::move(onUpdate),
						std::move(onSleep), std::move(onDestroy));

	const Script::CreateInfo info{
		.name = name,
		.onUpdate = onUpdate,
		.onStart = onCreate,
		.onSleep = onSleep,
		.onDestroy = onDestroy,
		.data = data,
	};

	return std::make_shared<Script>(info);
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
