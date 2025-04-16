#pragma once

#include "sol.hpp"
#include "scene.hpp"
#include "etna/etna_core.hpp"

using namespace etna;

class y3 {
public:
	y3(uint32_t width = 800, uint32_t height = 600);

	~y3();

	void run();

	void initLuaTypes();

	void removeScene();

	void switchScene(const std::string& name);

	static etna::Window* g_window;
	static sol::state lua;

private:
	sol::table y3_table;
	Scene* m_currScene{nullptr};
	Renderer* m_renderer{nullptr};
	std::unordered_map<std::string, etna::Scene*> m_scenes;

public:
	y3(const y3&) = delete;
	y3& operator=(const y3&) = delete;
	y3(y3&&) = delete;
	y3& operator=(y3&&) = delete;
};
