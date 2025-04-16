#pragma once

#include <functional>
#include <string>
#include "sol.hpp"

namespace etna {

struct _SceneNode;
class Scene;

struct Script {
	struct CreateInfo {
		std::string name;
		std::function<void(_SceneNode*, sol::table, float dt, Scene* const)>
			onUpdate;
		std::function<void(_SceneNode*, sol::table, Scene* const)> onCreate;
		std::function<void(_SceneNode*, sol::table, Scene* const)> onSleep;
		std::function<void(_SceneNode*, sol::table, Scene* const)> onDestroy;
		sol::table data;
	};

	Script(const CreateInfo& info) : m_info(info) {}

	CreateInfo m_info;
};

}
