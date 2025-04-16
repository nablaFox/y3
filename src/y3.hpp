#pragma once

#include "sol.hpp"
#include "scene.hpp"
#include "etna/etna_core.hpp"

class y3 {
public:
	y3(uint32_t width, uint32_t height);

	~y3();

	void run();

	void initLuaTypes();

	void removeScene();

	void switchScene(const std::string& name);

	void destroyScene(const std::string& name);

	void addGlobalScript(std::shared_ptr<etna::Script> script);

	void removeGlobalScript(const std::string& name);

	static etna::Window* g_window;
	static sol::state lua;

private:
	sol::table y3_table;
	etna::Renderer* m_renderer{nullptr};
	etna::Scene* m_currScene{nullptr};
	std::unordered_map<std::string, std::unique_ptr<etna::Scene>> m_scenes;
	std::unordered_map<std::string, etna::ScriptHandle> m_globalScripts;

public:
	y3(const y3&) = delete;
	y3& operator=(const y3&) = delete;
	y3(y3&&) = delete;
	y3& operator=(y3&&) = delete;
};
