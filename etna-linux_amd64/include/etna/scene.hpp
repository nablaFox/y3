#pragma once

#include <unordered_map>
#include "scene_graph.hpp"
#include "renderer.hpp"

namespace etna {

struct SceneRenderInfo {
	Viewport viewport;
	Color ambient{WHITE};
};

class Scene {
public:
	// Note: if we use a dynamic amount of them we need an SSBO
	static constexpr uint32_t MAX_LIGHTS = 16;

	Scene();
	~Scene();

	SceneNode addNode(SceneNode, const Transform& = {}, std::string = "");
	MeshNode addMesh(MeshNode, const Transform& = {});
	CameraNode addCamera(CameraNode, const Transform& = {});

	MeshNode createMeshNode(const CreateMeshNodeInfo&);
	CameraNode createCameraNode(const CreateCameraNodeInfo&);
	LightNode createLightNode(const DirectionalLight::CreateInfo&);

	SceneNode getNode(const std::string& name) const;
	void removeNode(const std::string& name);

	MeshNode getMesh(const std::string& name) const;
	CameraNode getCamera(const std::string& name) const;
	LightNode getLight(const std::string& name) const;

	void render(Renderer&, const CameraNode&, const SceneRenderInfo& = {});

public:
	const std::unordered_map<std::string, SceneNode>& getNodes() const;

	const std::vector<MeshNode>& getMeshes() const;
	const std::vector<LightNode>& getLights() const;
	const std::vector<CameraNode> getCameras() const;

	void print() const;

private:
	std::unordered_map<std::string, SceneNode> m_roots;

	void addNodeHelper(SceneNode node, const Transform& transform);
	void updateLights();

	ignis::BufferId m_sceneBuffer{IGNIS_INVALID_BUFFER_ID};
	ignis::BufferId m_lightsBuffer{IGNIS_INVALID_BUFFER_ID};

	// PONDER: if needed provide a method to explicitly invalidate cache
	mutable bool m_meshCacheDirty{true};
	mutable std::vector<MeshNode> m_meshCache;

	mutable bool m_lightCacheDirty{true};
	mutable std::vector<LightNode> m_lightCache;

	struct SceneData {
		Color ambient;
		ignis::BufferId lights;
		uint32_t lightCount;
	};

public:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
	Scene(Scene&&) = delete;
	Scene& operator=(Scene&&) = delete;
};

}  // namespace etna
