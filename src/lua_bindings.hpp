#include "script.hpp"
#include "scene_graph.hpp"

std::shared_ptr<etna::Script> create_script_from_file(std::string name,
													  sol::table data);

std::shared_ptr<etna::Script> create_script(sol::table scriptTable);

etna::SceneNode create_mesh(sol::table params);

etna::SceneNode create_camera(sol::table params);

etna::MaterialHandle create_grid_material(sol::table params);
