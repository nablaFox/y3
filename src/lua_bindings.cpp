#include "y3.hpp"

using namespace etna;

static Transform getTransform(sol::table params) {
	if (params["transform"].is<Transform>()) {
		return params["transform"];
	}

	Vec3 defaultPosition{0, 0, 0};
	Vec3 defaultScale{1, 1, 1};

	const Transform transform{
		.position = params.get_or("position", defaultPosition),
		.yaw = params.get_or("yaw", 0.0f),
		.pitch = params.get_or("pitch", 0.0f),
		.roll = params.get_or("roll", 0.0f),
		.scale = params.get_or("scale", defaultScale),
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

ScriptHandle create_script(sol::table scriptTable) {
	sol::function update = scriptTable["update"];
	sol::function start = scriptTable["start"];
	sol::function sleep = scriptTable["sleep"];
	sol::function destroy = scriptTable["destroy"];

	Script::HookFunc onCreate;
	Script::UpdateFunc onUpdate;
	Script::HookFunc onSleep;
	Script::HookFunc onDestroy;

	if (update.valid()) {
		onUpdate = [update](float dt, _SceneNode* node, sol::table data,
							Scene* scene) {
			sol::protected_function_result result = update(dt, node, data, scene);
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

	const Script::CreateInfo info{
		.name = scriptTable["name"],
		.onUpdate = onUpdate,
		.onStart = onCreate,
		.onSleep = onSleep,
		.onDestroy = onDestroy,
		.data = scriptTable["data"].get_or(sol::table()),
	};

	return std::make_shared<Script>(info);
}

SceneNode create_mesh(sol::table params) {
	scene::MeshNodeCreateInfo info{
		.name = params["name"],
		.mesh = params["mesh"].get_or<MeshHandle>(nullptr),
		.material = params["material"].get_or<MaterialHandle>(nullptr),
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
	Color defaultColor{WHITE};
	Color defaultGridColor{BLACK};

	engine::GridMaterialParams gridParams{
		.color = params["color"].get_or(defaultColor),
		.gridColor = params["gridColor"].get_or(defaultGridColor),
		.gridSpacing = params["gridSpacing"].get_or(1.0f),
		.thickness = params["thickness"].get_or(0.1f),
	};

	return engine::createGridMaterial(gridParams);
}

// TEMP: transparent materials will be better handled in the future
MaterialHandle create_grid_material_transparent(sol::table params) {
	Color defaultColor{WHITE};
	Color defaultGridColor{BLACK};

	engine::GridMaterialParams gridParams{
		.color = params["color"].get_or(defaultColor),
		.gridColor = params["gridColor"].get_or(defaultGridColor),
		.gridSpacing = params["gridSpacing"].get_or(1.0f),
		.thickness = params["thickness"].get_or(0.1f),
	};

	return engine::createTransparentGridMaterial(gridParams);
}
//

MaterialTemplateHandle create_material_template(sol::table params) {
	std::vector<RawShader> rawShaders;
	std::vector<std::string> shaders;

	if (params["shaders"].is<sol::table>()) {
		sol::table shaderTable = params["shaders"];

		for (const auto& kv : shaderTable) {
			sol::object value = kv.second;

			if (value.is<RawShader>()) {
				rawShaders.push_back(value.as<RawShader>());
			} else if (value.is<std::string>()) {
				shaders.push_back(value.as<std::string>());
			}
		}
	}

	const MaterialTemplate::CreateInfo info{
		.shaders = shaders,
		.rawShaders = rawShaders,
		.enableDepth = params["enableDepth"].get_or(true),
		.transparency = params["transparency"].get_or(false),
		.polygonMode = params["polygonMode"].get_or(VK_POLYGON_MODE_FILL),
		.lineWidth = params["lineWidth"].get_or(1.0f),
		.samples = params["samples"].get_or(0u),
	};

	return MaterialTemplate::create(info);
}

template <typename T>
static void append_bytes(std::vector<uint8_t>& buf, const T& v) {
	const uint8_t* p = reinterpret_cast<const uint8_t*>(&v);
	buf.insert(buf.end(), p, p + sizeof(T));
}

MaterialHandle create_material(sol::table params) {
	sol::object raw = params["params"];
	if (!raw.valid() || raw.get_type() != sol::type::table) {
		throw std::runtime_error{
			"create_material: expected params.params to be a table"};
	}

	sol::table list = raw.as<sol::table>();
	std::vector<uint8_t> buf;

	for (const auto& kv : raw.as<sol::table>()) {
		sol::object v = kv.second;

		if (v.is<float>()) {
			append_bytes(buf, v.as<float>());
		} else if (v.is<int>()) {
			append_bytes(buf, v.as<int>());
		} else if (v.is<bool>()) {
			buf.push_back(v.as<bool>() ? 1 : 0);
		} else if (v.is<Vec3>()) {
			Vec3 vec = v.as<Vec3>();
			append_bytes(buf, vec[0]);
			append_bytes(buf, vec[1]);
			append_bytes(buf, vec[2]);
		} else if (v.is<Vec4>()) {
			Vec4 vec = v.as<Vec4>();
			append_bytes(buf, vec[0]);
			append_bytes(buf, vec[1]);
			append_bytes(buf, vec[2]);
			append_bytes(buf, vec[3]);
		} else if (v.is<Color>()) {
			Color c = v.as<Color>();
			append_bytes(buf, c.r);
			append_bytes(buf, c.g);
			append_bytes(buf, c.b);
			append_bytes(buf, c.a);
		} else {
			throw std::runtime_error{"Unsupported param type in material params"};
		}
	}

	Material::CreateInfo info{
		.templateHandle = params["template"],
		.paramsSize = buf.size(),
		.params = buf.data(),
	};

	return Material::create(info);
}

void y3::initLuaBindings() {
	// scene management
	y3_table.set_function("add_global_script",
						  sol::overload([this](ScriptHandle script) {
							  this->addGlobalScript(script);
						  }));

	y3_table.set_function("remove_global_script", [this](const std::string& name) {
		removeGlobalScript(name);
	});

	y3_table.set_function("switch_scene", [this](const std::string& sceneName) {
		switchScene(sceneName);
	});

	y3_table.set_function("destroy_scene", [this](const std::string& sceneName) {
		destroyScene(sceneName);
	});

	// scene graph
	y3_table.set_function("create_script", &create_script);
	y3_table.set_function("create_camera", &create_camera);
	y3_table.set_function("create_mesh", &create_mesh);
	y3_table.set_function("create_root", &scene::createRoot);

	// materials
	y3_table.set_function("create_material_template", &create_material_template);
	y3_table.set_function("create_material", &create_material);
	y3_table.set_function("create_grid_material", &create_grid_material);
	y3_table.set_function("create_grid_material_transparent",
						  &create_grid_material_transparent);
	y3_table.set_function("create_color_material", &engine::createColorMaterial);
	y3_table.set_function("create_point_material", &engine::createPointMaterial);
	y3_table.set_function("default_vert_shader", &engine::getDefaultVertShader);

	// primitives
	y3_table.set_function("get_sphere", engine::getSphere);
	y3_table.set_function("get_cube", engine::getCube);
	y3_table.set_function("get_pyramid", engine::getPyramid);
	y3_table.set_function("get_quad", engine::getQuad);

	// window
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
