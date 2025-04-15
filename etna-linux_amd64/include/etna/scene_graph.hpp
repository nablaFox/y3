#include <memory>
#include "light.hpp"
#include "transform.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "camera.hpp"

namespace etna {

struct _SceneNode;
struct _MeshNode;
struct _CameraNode;
struct _LightNode;

using SceneNode = std::shared_ptr<_SceneNode>;
using MeshNode = std::shared_ptr<_MeshNode>;
using CameraNode = std::shared_ptr<_CameraNode>;
using LightNode = std::shared_ptr<_LightNode>;

struct CreateMeshNodeInfo {
	std::string name;
	MeshHandle mesh;
	Transform transform;
	MaterialHandle material{nullptr};
	ignis::BufferId instanceBuffer{IGNIS_INVALID_BUFFER_ID};
	uint32_t instanceCount{1};
};

struct CreateCameraNodeInfo {
	std::string name;
	Transform transform;
	Camera::CreateInfo cameraInfo;
};

struct _SceneNode {
	enum class Type {
		ROOT,
		MESH,
		CAMERA,
		LIGHT,
	};

	_SceneNode(Type, const std::string&, const Transform&);

	SceneNode add(SceneNode);

	MeshNode createMeshNode(const CreateMeshNodeInfo&);
	CameraNode createCameraNode(const CreateCameraNodeInfo&);
	LightNode createLightNode(const DirectionalLight::CreateInfo&);

	_SceneNode* getParent() const { return m_parent; }

	void remove();

	bool isRoot() const { return m_parent == nullptr; }

	const Transform& getTransform() const { return m_transform; }

	Mat4 getWorldMatrix() const { return m_worldMatrix; }

	void updateTransform(const Transform&);

	void updatePosition(const Vec3&);

	void translate(const Vec3&);

	void rotate(float yaw, float pitch, float roll);

	const std::string& getName() const { return m_name; }

	Type getType() const { return m_type; }

	const std::vector<SceneNode>& getChildren() const { return m_children; }

#ifndef NDEBUG
	void print() const;

	const char* getTypeLabel() const;
#endif

protected:
	Transform m_transform;
	Mat4 m_worldMatrix{Mat4::identity()};
	std::string m_name;
	Type m_type;

	_SceneNode* m_parent{nullptr};
	std::vector<SceneNode> m_children;

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
};

struct _LightNode : public _SceneNode {
	using _SceneNode::_SceneNode;

	std::shared_ptr<DirectionalLight> light;
};

namespace scene {

SceneNode createRoot(const std::string&, const Transform& = {});

SceneNode loadFromFile(const std::string& path);

MeshNode createMeshNode(const CreateMeshNodeInfo&);

CameraNode createCameraNode(const CreateCameraNodeInfo&);

LightNode createLightNode(const DirectionalLight::CreateInfo&);

SceneNode find(const std::string& name, const SceneNode& root);

std::vector<MeshNode> getMeshes(const SceneNode&);
std::vector<CameraNode> getCameras(const SceneNode&);
std::vector<LightNode> getLights(const SceneNode&);

}  // namespace scene

}  // namespace etna
