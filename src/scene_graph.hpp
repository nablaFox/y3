#pragma once

#include "etna/light.hpp"
#include "etna/transform.hpp"
#include "etna/mesh.hpp"
#include "etna/material.hpp"
#include "etna/camera.hpp"
#include "etna/renderer.hpp"
#include "script.hpp"

namespace etna {

struct _SceneNode;
using SceneNode = std::shared_ptr<_SceneNode>;

struct _SceneNode {
	enum class Type {
		ROOT,
		MESH,
		CAMERA,
		LIGHT,
	};

	_SceneNode(Type, const std::string&);

	SceneNode add(SceneNode);

	_SceneNode* getParent() const { return m_parent; }

	void remove();

	void addScript(const Script*);

	void applyCreateScripts(Scene*);

	void applyUpdateScripts(Scene*);

	void applyDestroyScripts(Scene*);

	void* getScriptData(const std::string& name) const;

	bool isRoot() const { return m_parent == nullptr; }

	const Transform& getTransform() const { return m_transform; }

	Mat4 getWorldMatrix() const { return m_worldMatrix; }

	void updateTransform(const Transform&);

	void updatePosition(const Vec3&);

	void translate(const Vec3&);

	void rotate(float yaw, float pitch, float roll);

	std::string getName() const { return m_name; }

	Type getType() const { return m_type; }

	const std::vector<SceneNode>& getChildren() const { return m_children; }

#ifndef NDEBUG
	void print() const;

	const char* getTypeLabel() const;
#endif

protected:
	Transform m_transform{};
	Mat4 m_worldMatrix{Mat4::identity()};
	std::string m_name;
	Type m_type;

	_SceneNode* m_parent{nullptr};
	std::vector<SceneNode> m_children;
	std::vector<const Script*> m_scripts;

	void updateChildrenTransform(const Mat4&);
};

struct _MeshNode : public _SceneNode {
	using _SceneNode::_SceneNode;

	~_MeshNode();

	MaterialHandle material;
	MeshHandle mesh;
	ignis::BufferId instanceBuffer;
	uint32_t instanceCount;
};

struct _CameraNode : public _SceneNode {
	using _SceneNode::_SceneNode;

	std::shared_ptr<Camera> camera;
	Viewport viewport;
	RenderTarget* renderTarget{nullptr};
};

struct _LightNode : public _SceneNode {
	using _SceneNode::_SceneNode;

	std::shared_ptr<DirectionalLight> light;
};

using MeshNode = std::shared_ptr<_MeshNode>;
using CameraNode = std::shared_ptr<_CameraNode>;
using LightNode = std::shared_ptr<_LightNode>;

namespace scene {

struct MeshNodeCreateInfo {
	std::string name;
	MeshHandle mesh;
	MaterialHandle material{nullptr};
	Transform transform;
	std::vector<Script*> scripts;
	ignis::BufferId instanceBuffer{IGNIS_INVALID_BUFFER_ID};
	uint32_t instanceCount{1};
};

struct CameraNodeCreateInfo {
	std::string name;
	Camera::CreateInfo cameraInfo;
	Viewport viewport{0, 0, 0, 0};
	RenderTarget* renderTarget{nullptr};
	Transform transform;
	std::vector<Script*> scripts;
};

SceneNode createRoot(const std::string&, const Transform& = {});

SceneNode loadFromFile(const std::string& path);

MeshNode createMeshNode(const MeshNodeCreateInfo&);

CameraNode createCameraNode(const CameraNodeCreateInfo&);

LightNode createLightNode(const DirectionalLight::CreateInfo&);

SceneNode find(const std::string& name, const SceneNode& root);

std::vector<MeshNode> getMeshes(const SceneNode&);
std::vector<CameraNode> getCameras(const SceneNode&);
std::vector<LightNode> getLights(const SceneNode&);

}  // namespace scene

}  // namespace etna
