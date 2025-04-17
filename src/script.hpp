#pragma once

#include <functional>
#include <string>
#include "sol.hpp"

namespace etna {

struct _SceneNode;
class Scene;

struct Script {
	using HookFunc = std::function<void(_SceneNode*, sol::table, Scene* const)>;

	using UpdateFunc =
		std::function<void(float dt, _SceneNode*, sol::table, Scene* const)>;

	struct CreateInfo {
		std::string name;
		UpdateFunc onUpdate;
		HookFunc onStart;
		HookFunc onSleep;
		HookFunc onDestroy;
		sol::table data;
	};

	Script(const CreateInfo& info) : m_info(info) {}

	CreateInfo m_info;
};

using ScriptHandle = std::shared_ptr<Script>;

}
