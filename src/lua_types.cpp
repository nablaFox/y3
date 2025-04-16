#include "y3.hpp"

void y3::initLuaTypes() {
	lua.new_usertype<Vec3>(
		"Vec3", sol::constructors<Vec3(float, float, float)>(),			  //
		"x", sol::property([](const Vec3& v) -> float { return v[0]; }),  //
		"y", sol::property([](const Vec3& v) -> float { return v[1]; }),  //
		"z", sol::property([](const Vec3& v) -> float { return v[2]; }),  //
		sol::meta_function::addition,
		[](const Vec3& a, const Vec3& b) {
			return Vec3(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
		},
		sol::meta_function::subtraction,
		[](const Vec3& a, const Vec3& b) {
			return Vec3(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
		},
		sol::meta_function::multiplication,
		sol::overload(
			[](const Vec3& a, float scalar) {
				return Vec3(a[0] * scalar, a[1] * scalar, a[2] * scalar);
			},
			[](float scalar, const Vec3& a) {
				return Vec3(a[0] * scalar, a[1] * scalar, a[2] * scalar);
			}));

	lua.new_usertype<Transform>("Transform",					   //
								"position", &Transform::position,  //
								"yaw", &Transform::yaw,			   //
								"pitch", &Transform::pitch,		   //
								"roll", &Transform::roll,		   //
								"scale", &Transform::scale,		   //
								"forward", &Transform::forward,	   //
								"right", &Transform::right,		   //
								"up", &Transform::up);

	lua.new_usertype<_CameraNode>("CameraNode", sol::base_classes,
								  sol::bases<_SceneNode>());

	lua.new_usertype<_MeshNode>("MeshNode", sol::base_classes,
								sol::bases<_SceneNode>());

	lua.new_usertype<_SceneNode>("SceneNode",						   //
								 "get_name", &_SceneNode::getName,	   //
								 "translate", &_SceneNode::translate,  //
								 "rotate", &_SceneNode::rotate,		   //
								 "get_transform", &_SceneNode::getTransform,
								 "update_transform", &_SceneNode::updateTransform);

	lua.new_usertype<etna::Color>(
		"Color", sol::constructors<>(),	   //
		"r", &etna::Color::r,			   //
		"g", &etna::Color::g,			   //
		"b", &etna::Color::b,			   //
		"a", &etna::Color::a, "setAlpha",  //
		&etna::Color::setAlpha, sol::meta_function::multiplication,
		[](const etna::Color& c, float f) { return c * f; });

	lua["WHITE"] = etna::WHITE;
	lua["BLACK"] = etna::BLACK;
	lua["RED"] = etna::RED;
	lua["GREEN"] = etna::GREEN;
	lua["BLUE"] = etna::BLUE;
	lua["PURPLE"] = etna::PURPLE;
	lua["CELESTE"] = etna::CELESTE;
	lua["YELLOW"] = etna::YELLOW;

	lua["KEY_A"] = etna::KEY_A;
	lua["KEY_B"] = etna::KEY_B;
	lua["KEY_C"] = etna::KEY_C;
	lua["KEY_D"] = etna::KEY_D;
	lua["KEY_E"] = etna::KEY_E;
	lua["KEY_F"] = etna::KEY_F;
	lua["KEY_G"] = etna::KEY_G;
	lua["KEY_H"] = etna::KEY_H;
	lua["KEY_I"] = etna::KEY_I;
	lua["KEY_J"] = etna::KEY_J;
	lua["KEY_K"] = etna::KEY_K;
	lua["KEY_L"] = etna::KEY_L;
	lua["KEY_M"] = etna::KEY_M;
	lua["KEY_N"] = etna::KEY_N;
	lua["KEY_O"] = etna::KEY_O;
	lua["KEY_P"] = etna::KEY_P;
	lua["KEY_Q"] = etna::KEY_Q;
	lua["KEY_R"] = etna::KEY_R;
	lua["KEY_S"] = etna::KEY_S;
	lua["KEY_T"] = etna::KEY_T;
	lua["KEY_U"] = etna::KEY_U;
	lua["KEY_V"] = etna::KEY_V;
	lua["KEY_W"] = etna::KEY_W;
	lua["KEY_X"] = etna::KEY_X;
	lua["KEY_Y"] = etna::KEY_Y;
	lua["KEY_Z"] = etna::KEY_Z;
	lua["KEY_0"] = etna::KEY_0;
	lua["KEY_1"] = etna::KEY_1;
	lua["KEY_2"] = etna::KEY_2;
	lua["KEY_3"] = etna::KEY_2;
	lua["KEY_4"] = etna::KEY_4;
	lua["KEY_5"] = etna::KEY_5;
	lua["KEY_6"] = etna::KEY_6;
	lua["KEY_7"] = etna::KEY_7;
	lua["KEY_8"] = etna::KEY_8;
	lua["KEY_9"] = etna::KEY_9;
	lua["KEY_F1"] = etna::KEY_F1;
	lua["KEY_F2"] = etna::KEY_F2;
	lua["KEY_F3"] = etna::KEY_F3;
	lua["KEY_F4"] = etna::KEY_F4;
	lua["KEY_F5"] = etna::KEY_F5;
	lua["KEY_F6"] = etna::KEY_F6;
	lua["KEY_F7"] = etna::KEY_F7;
	lua["KEY_F8"] = etna::KEY_F8;
	lua["KEY_F9"] = etna::KEY_F9;
	lua["KEY_F10"] = etna::KEY_F10;
	lua["KEY_F11"] = etna::KEY_F11;
	lua["KEY_F12"] = etna::KEY_F12;
	lua["KEY_LEFT"] = etna::KEY_LEFT;
	lua["KEY_SPACE"] = etna::KEY_SPACE;
	lua["KEY_LSHIFT"] = etna::KEY_LEFT_SHIFT;
	lua["KEY_RIGHT"] = etna::KEY_RIGHT;
	lua["KEY_UP"] = etna::KEY_UP;
	lua["KEY_DOWN"] = etna::KEY_DOWN;
	lua["KEY_ENTER"] = etna::KEY_ENTER;
	lua["KEY_BACKSPACE"] = etna::KEY_BACKSPACE;
	lua["KEY_TAB"] = etna::KEY_TAB;
}
