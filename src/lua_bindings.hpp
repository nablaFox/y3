#include "y3.hpp"

static inline Transform getTransform(sol::table params) {
	Vec3 defaultPosition{0, 0, 0};

	const Transform transform{
		.position = params.get_or("position", defaultPosition),
		.yaw = params.get_or("yaw", 0.0f),
		.pitch = params.get_or("pitch", 0.0f),
		.roll = params.get_or("roll", 0.0f),
	};

	return transform;
}

static inline std::vector<Script*> getScripts(sol::table params) {
	if (params["scripts"].is<Script*>()) {
		return {params["scripts"]};
	}

	if (params["scripts"].is<sol::table>()) {
		std::vector<Script*> scripts;

		sol::table scriptTable = params["scripts"];

		for (auto& kv : scriptTable) {
			sol::object value = kv.second;
			if (value.is<Script*>()) {
				scripts.push_back(value.as<Script*>());
			}
		}

		return scripts;
	}

	return {};
}

inline etna::Script* script(std::string name, sol::table data) {
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

	sol::function update = y3::lua["Update"];
	sol::function start = y3::lua["Start"];
	sol::function sleep = y3::lua["Sleep"];

	if (!update.valid() || !start.valid() || !sleep.valid()) {
		std::cerr
			<< "Script \"" << path
			<< "\" does not define all required functions (update, start, sleep)."
			<< std::endl;
		return nullptr;
	}

	std::function<void(etna::_SceneNode*, sol::table, float, etna::Scene* const)>
		onUpdate = [update](etna::_SceneNode* node, sol::table data, float dt,
							etna::Scene* scene) {
			sol::protected_function_result result = update(node, data, dt, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in update: " << err.what() << std::endl;
			}
		};

	std::function<void(etna::_SceneNode*, sol::table, etna::Scene* const)> onCreate =
		[start](etna::_SceneNode* node, sol::table data, etna::Scene* scene) {
			sol::protected_function_result result = start(node, data, scene);
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Error in start: " << err.what() << std::endl;
			}
		};

	std::function<void(etna::_SceneNode*, sol::table, etna::Scene* const)>
		onDestroy =
			[sleep](etna::_SceneNode* node, sol::table data, etna::Scene* scene) {
				sol::protected_function_result result = sleep(node, data, scene);
				if (!result.valid()) {
					sol::error err = result;
					std::cerr << "Error in sleep: " << err.what() << std::endl;
				}
			};

	return new etna::Script({
		.name = name,
		.onUpdate = onUpdate,
		.onCreate = onCreate,
		.onDestroy = onDestroy,
		.data = data,
	});
}

inline SceneNode create_mesh(sol::table params) {
	scene::MeshNodeCreateInfo info{
		.name = params["name"],
		.mesh = params["mesh"],
		.material = params["material"],
		.transform = getTransform(params),
		.scripts = getScripts(params),
	};

	return scene::createMeshNode(info);
}

inline SceneNode create_camera(sol::table params) {
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

inline MaterialHandle create_grid_material(sol::table params) {
	engine::GridMaterialParams gridParams{
		.color = params["color"],
		.gridColor = params["gridColor"],
		.gridSpacing = params["gridSpacing"],
		.thickness = params["thickness"],
	};

	return engine::createGridMaterial(gridParams);
}
