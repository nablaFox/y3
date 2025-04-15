#include "scene.hpp"
#include "etna/engine.hpp"

using namespace etna;
using namespace ignis;

Scene::Scene()
	: m_sceneBuffer(_device.createUBO(sizeof(SceneData))),
	  m_lightsBuffer(
		  _device.createUBO(sizeof(ignis::BufferId) * Scene::MAX_LIGHTS)) {}

Scene::~Scene() {
	_device.destroyBuffer(m_sceneBuffer);
	_device.destroyBuffer(m_lightsBuffer);
}

SceneNode Scene::addNode(SceneNode node,
						 const Transform& transform,
						 std::string name) {
	if (node == nullptr)
		return nullptr;

	node->translate(transform.position);
	node->rotate(transform.yaw, transform.pitch, transform.roll);

	m_roots[name.empty() ? node->getName() : name] = node;

	if (node->getType() == _SceneNode::Type::LIGHT) {
		m_lightCacheDirty = true;

		std::vector<ignis::BufferId> lights;

		for (const auto& light : getLights()) {
			if (light->light->getIntensity() > 0) {
				lights.push_back(light->light->getDataBuffer());
			}
		}

		if (lights.size() > Scene::MAX_LIGHTS) {
			throw std::runtime_error("Exceeded maximum number of lights per scene");
		}

		if (lights.size() > 0) {
			_device.updateBuffer(m_lightsBuffer, lights.data());
		}
	}

	if (node->getType() == _SceneNode::Type::MESH) {
		m_meshCacheDirty = true;
	}

	return node;
}

MeshNode Scene::addMesh(MeshNode node, const Transform& transform) {
	return std::static_pointer_cast<_MeshNode>(addNode(node, transform));
}

CameraNode Scene::addCamera(CameraNode node, const Transform& transform) {
	return std::static_pointer_cast<_CameraNode>(addNode(node, transform));
}

MeshNode Scene::createMeshNode(const CreateMeshNodeInfo& info) {
	return addMesh(scene::createMeshNode(info));
}

CameraNode Scene::createCameraNode(const CreateCameraNodeInfo& info) {
	return addCamera(scene::createCameraNode(info));
}

LightNode Scene::createLightNode(const DirectionalLight::CreateInfo& info) {
	LightNode lightNode = scene::createLightNode(info);

	m_roots[info.name] = lightNode;

	addNode(lightNode);

	return lightNode;
}

SceneNode Scene::getNode(const std::string& name) const {
	const std::string rootName = name.substr(0, name.find('/'));
	const std::string childName = name.substr(name.find('/') + 1);

	auto it = m_roots.find(rootName);

	if (it != m_roots.end() && it->first == name) {
		return it->second;
	}

	if (it != m_roots.end()) {
		return scene::find(childName, it->second);
	}

	return nullptr;
}

void Scene::removeNode(const std::string& name) {
	SceneNode node = getNode(name);

	if (node == nullptr) {
		return;
	}

	if (!node->isRoot()) {
		return node->remove();
	}

	m_roots.erase(name);
}

MeshNode Scene::getMesh(const std::string& name) const {
	SceneNode node = getNode(name);

	if (node != nullptr && node->getType() == _SceneNode::Type::MESH) {
		return std::static_pointer_cast<_MeshNode>(node);
	}

	return nullptr;
}

CameraNode Scene::getCamera(const std::string& name) const {
	SceneNode node = getNode(name);

	if (node != nullptr && node->getType() == _SceneNode::Type::CAMERA) {
		return std::static_pointer_cast<_CameraNode>(node);
	}

	return nullptr;
}

LightNode Scene::getLight(const std::string& name) const {
	SceneNode node = getNode(name);

	if (node != nullptr && node->getType() == _SceneNode::Type::LIGHT) {
		return std::static_pointer_cast<_LightNode>(node);
	}

	return nullptr;
}

void Scene::render(Renderer& renderer,
				   const CameraNode& cameraNode,
				   const SceneRenderInfo& info) {
	Viewport vp{info.viewport};

	if (vp.width == 0) {
		vp.x = 0;
		vp.width = (float)renderer.getRenderTarget().getExtent().width;
	}

	if (vp.height == 0) {
		vp.y = 0;
		vp.height = (float)renderer.getRenderTarget().getExtent().height;
	}

	const SceneData sceneData{
		.ambient = info.ambient,
		.lights = m_lightsBuffer,
		.lightCount = static_cast<uint32_t>(getLights().size()),
	};

	_device.updateBuffer(m_sceneBuffer, &sceneData);

	cameraNode->camera->updateAspect(vp.width / vp.height);

	for (const auto& meshNode : getMeshes()) {
		if (meshNode->mesh == nullptr)
			continue;

		const MaterialHandle material = meshNode->material;
		const MeshHandle mesh = meshNode->mesh;
		const Mat4 worldMatrix = meshNode->getWorldMatrix();

		renderer.draw({
			.mesh = mesh,
			.material = material,
			.transform = worldMatrix,
			.viewport = vp,
			.buff1 = m_sceneBuffer,
			.buff2 = cameraNode->camera->getDataBuffer(),
			.instanceBuffer = meshNode->instanceBuffer,
			.instanceCount = meshNode->instanceCount,
		});
	}
}

const std::unordered_map<std::string, SceneNode>& Scene::getNodes() const {
	return m_roots;
}

// TEMP: add caching
const std::vector<MeshNode>& Scene::getMeshes() const {
	if (!m_meshCacheDirty)
		return m_meshCache;

	m_meshCache.clear();

	for (const auto& [_, root] : m_roots) {
		std::vector<MeshNode> childMeshes = scene::getMeshes(root);
		m_meshCache.insert(m_meshCache.end(), childMeshes.begin(),
						   childMeshes.end());
	}

	m_meshCacheDirty = false;

	return m_meshCache;
}

const std::vector<CameraNode> Scene::getCameras() const {
	std::vector<CameraNode> cameras;

	for (const auto& [_, root] : m_roots) {
		std::vector<CameraNode> childCameras = scene::getCameras(root);
		cameras.insert(cameras.end(), childCameras.begin(), childCameras.end());
	}

	return cameras;
}

const std::vector<LightNode>& Scene::getLights() const {
	if (!m_lightCacheDirty)
		return m_lightCache;

	m_lightCache.clear();

	for (const auto& [_, root] : m_roots) {
		std::vector<LightNode> childLights = scene::getLights(root);
		m_lightCache.insert(m_lightCache.end(), childLights.begin(),
							childLights.end());
	}

	m_lightCacheDirty = false;

	return m_lightCache;
}

void Scene::print() const {
#ifndef NDEBUG

	for (const auto& [name, node] : m_roots) {
		std::cout << "(" << name << ") ";
		node->print();
	}

	std::cout << std::endl;
#endif
}
