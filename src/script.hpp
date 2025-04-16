#pragma once

#include <functional>
#include <string>

namespace etna {

struct _SceneNode;
class Scene;

struct Script {
	struct CreateInfo {
		std::string name;
		std::function<void(_SceneNode*, void*, float dt, Scene* const)> onUpdate;
		std::function<void(_SceneNode*, void*, Scene* const)> onCreate;
		std::function<void(_SceneNode*, void*, Scene* const)> onDestroy;
		void* const data{nullptr};
	};

	Script(const CreateInfo& info) : m_info(info) {}

	CreateInfo m_info;
};

}
