#include "y3.hpp"

using namespace etna;

void y3::initLuaTypes() {
	m_lua.new_usertype<Vec3>(
		"Vec3", sol::constructors<Vec3(float, float, float), Vec3(float)>(),  //
		"x", sol::property([](const Vec3& v) -> float { return v[0]; }),	  //
		"y", sol::property([](const Vec3& v) -> float { return v[1]; }),	  //
		"z", sol::property([](const Vec3& v) -> float { return v[2]; }),	  //
		sol::meta_function::addition,
		[](const Vec3& a, const Vec3& b) { return Vec3{a + b}; },
		sol::meta_function::subtraction,
		[](const Vec3& a, const Vec3& b) { return Vec3{a - b}; },
		sol::meta_function::division,
		[](const Vec3& a, float scalar) { return Vec3{a / scalar}; },

		sol::meta_function::multiplication,
		sol::overload([](const Vec3& a, float scalar) { return Vec3{a * scalar}; },
					  [](float scalar, const Vec3& a) {
						  return Vec3(a[0] * scalar, a[1] * scalar, a[2] * scalar);
					  }));

	m_lua.new_usertype<Transform>(
		"Transform",  //
		sol::no_constructor, "new", sol::factories([](sol::table t) {
			Transform tr;

			if (auto p = t["position"].get_or<sol::optional<Vec3>>(Vec3{}))
				tr.position = *p;
			if (auto y = t["yaw"].get_or<sol::optional<float>>(0))
				tr.yaw = *y;
			if (auto p = t["pitch"].get_or<sol::optional<float>>(0))
				tr.pitch = *p;
			if (auto r = t["roll"].get_or<sol::optional<float>>(0))
				tr.roll = *r;
			if (auto s = t["scale"].get_or<sol::optional<Vec3>>(Vec3{1, 1, 1}))
				tr.scale = *s;

			return tr;
		}),
		"position", &Transform::position,  //
		"yaw", &Transform::yaw,			   //
		"pitch", &Transform::pitch,		   //
		"roll", &Transform::roll,		   //
		"scale", &Transform::scale,		   //
		"forward", &Transform::forward,	   //
		"right", &Transform::right,		   //
		"up", &Transform::up);

	m_lua.new_usertype<_CameraNode>("CameraNode", sol::base_classes,
									sol::bases<_SceneNode>());

	m_lua.new_usertype<_MeshNode>("MeshNode", sol::base_classes,
								  sol::bases<_SceneNode>());

	m_lua.new_usertype<_SceneNode>(
		"SceneNode",									   //
		"get_name", &_SceneNode::getName,				   //
		"translate", &_SceneNode::translate,			   //
		"rotate", &_SceneNode::rotate,					   //
		"get_transform", &_SceneNode::getTransform,		   //
		"update_transform", &_SceneNode::updateTransform,  //
		"update_position", &_SceneNode::updatePosition,	   //
		"get_script_data", &_SceneNode::getScriptData,	   //
		"add_script", &_SceneNode::addScript,			   //
		"add", &_SceneNode::add);

	m_lua.new_usertype<etna::Color>(
		"Color", sol::constructors<>(),		//
		"r", &etna::Color::r,				//
		"g", &etna::Color::g,				//
		"b", &etna::Color::b,				//
		"a", &etna::Color::a, "set_alpha",	//
		&etna::Color::setAlpha, sol::meta_function::multiplication,
		[](const etna::Color& c, float f) { return c * f; });

	m_lua["WHITE"] = etna::WHITE;
	m_lua["BLACK"] = etna::BLACK;
	m_lua["RED"] = etna::RED;
	m_lua["GREEN"] = etna::GREEN;
	m_lua["BLUE"] = etna::BLUE;
	m_lua["PURPLE"] = etna::PURPLE;
	m_lua["CELESTE"] = etna::CELESTE;
	m_lua["YELLOW"] = etna::YELLOW;
	m_lua["INVISIBLE"] = etna::INVISIBLE;

	m_lua["KEY_A"] = etna::KEY_A;
	m_lua["KEY_B"] = etna::KEY_B;
	m_lua["KEY_C"] = etna::KEY_C;
	m_lua["KEY_D"] = etna::KEY_D;
	m_lua["KEY_E"] = etna::KEY_E;
	m_lua["KEY_F"] = etna::KEY_F;
	m_lua["KEY_G"] = etna::KEY_G;
	m_lua["KEY_H"] = etna::KEY_H;
	m_lua["KEY_I"] = etna::KEY_I;
	m_lua["KEY_J"] = etna::KEY_J;
	m_lua["KEY_K"] = etna::KEY_K;
	m_lua["KEY_L"] = etna::KEY_L;
	m_lua["KEY_M"] = etna::KEY_M;
	m_lua["KEY_N"] = etna::KEY_N;
	m_lua["KEY_O"] = etna::KEY_O;
	m_lua["KEY_P"] = etna::KEY_P;
	m_lua["KEY_Q"] = etna::KEY_Q;
	m_lua["KEY_R"] = etna::KEY_R;
	m_lua["KEY_S"] = etna::KEY_S;
	m_lua["KEY_T"] = etna::KEY_T;
	m_lua["KEY_U"] = etna::KEY_U;
	m_lua["KEY_V"] = etna::KEY_V;
	m_lua["KEY_W"] = etna::KEY_W;
	m_lua["KEY_X"] = etna::KEY_X;
	m_lua["KEY_Y"] = etna::KEY_Y;
	m_lua["KEY_Z"] = etna::KEY_Z;
	m_lua["KEY_0"] = etna::KEY_0;
	m_lua["KEY_1"] = etna::KEY_1;
	m_lua["KEY_2"] = etna::KEY_2;
	m_lua["KEY_3"] = etna::KEY_2;
	m_lua["KEY_4"] = etna::KEY_4;
	m_lua["KEY_5"] = etna::KEY_5;
	m_lua["KEY_6"] = etna::KEY_6;
	m_lua["KEY_7"] = etna::KEY_7;
	m_lua["KEY_8"] = etna::KEY_8;
	m_lua["KEY_9"] = etna::KEY_9;
	m_lua["KEY_F1"] = etna::KEY_F1;
	m_lua["KEY_F2"] = etna::KEY_F2;
	m_lua["KEY_F3"] = etna::KEY_F3;
	m_lua["KEY_F4"] = etna::KEY_F4;
	m_lua["KEY_F5"] = etna::KEY_F5;
	m_lua["KEY_F6"] = etna::KEY_F6;
	m_lua["KEY_F7"] = etna::KEY_F7;
	m_lua["KEY_F8"] = etna::KEY_F8;
	m_lua["KEY_F9"] = etna::KEY_F9;
	m_lua["KEY_F10"] = etna::KEY_F10;
	m_lua["KEY_F11"] = etna::KEY_F11;
	m_lua["KEY_F12"] = etna::KEY_F12;
	m_lua["KEY_LEFT"] = etna::KEY_LEFT;
	m_lua["KEY_SPACE"] = etna::KEY_SPACE;
	m_lua["KEY_LSHIFT"] = etna::KEY_LEFT_SHIFT;
	m_lua["KEY_RIGHT"] = etna::KEY_RIGHT;
	m_lua["KEY_UP"] = etna::KEY_UP;
	m_lua["KEY_DOWN"] = etna::KEY_DOWN;
	m_lua["KEY_ENTER"] = etna::KEY_ENTER;
	m_lua["KEY_BACKSPACE"] = etna::KEY_BACKSPACE;
	m_lua["KEY_TAB"] = etna::KEY_TAB;
}
