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

inline SceneNode create_mesh(sol::table params) {
	scene::MeshNodeCreateInfo info{
		.name = params["name"],
		.mesh = params["mesh"],
		.material = params["material"],
		.transform = getTransform(params),
	};

	return scene::createMeshNode(info);
}

inline SceneNode create_camera(sol::table params) {
	Camera::CreateInfo defaultCameraInfo{};
	Viewport defaultViewport{};

	scene::CameraNodeCreateInfo info{
		.name = params["name"],
		.transform = getTransform(params),
		.cameraInfo = params["cameraInfo"].get_or(defaultCameraInfo),
		.viewport = params["viewport"].get_or(defaultViewport),
		.renderTarget = y3::g_window,
	};

	SceneNode node = scene::createCameraNode(info);

	if (params["scripts"].is<Script*>()) {
		Script* script = params["scripts"];
		node->addScript(script);
	}

	// TODO: handle also when the user passes more than one script

	return node;
}

inline etna::Script* create_script(sol::table params) {
	etna::Script::CreateInfo info;

	info.name = params["name"];

	if (params["update"].is<sol::function>()) {
		sol::function updateFunc = params["update"];

		info.onUpdate = updateFunc;
	}

	if (params["start"].is<sol::function>()) {
		sol::function onCreateFunc = params["start"];

		info.onCreate = onCreateFunc;
	}

	if (params["sleep"].is<sol::function>()) {
		sol::function onDestroyFunc = params["sleep"];

		info.onDestroy = onDestroyFunc;
	}

	info.data = params["data"];

	etna::Script* script = new etna::Script(info);

	return script;
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
