#include "y3.hpp"

void y3::initLuaTypes() {
	lua.new_usertype<Vec3>("Vec3", sol::constructors<Vec3(float, float, float)>());

	lua.new_usertype<Transform>("Transform",					   //
								"position", &Transform::position,  //
								"yaw", &Transform::yaw,			   //
								"pitch", &Transform::pitch,		   //
								"roll", &Transform::roll,		   //
								"scale", &Transform::scale);

	lua.new_usertype<_CameraNode>("CameraNode", sol::base_classes,
								  sol::bases<_SceneNode>());

	lua.new_usertype<_MeshNode>("MeshNode", sol::base_classes,
								sol::bases<_SceneNode>());

	lua.new_usertype<_SceneNode>("SceneNode",						   //
								 "get_name", &_SceneNode::getName,	   //
								 "translate", &_SceneNode::translate,  //
								 "rotate", &_SceneNode::rotate);

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
}
