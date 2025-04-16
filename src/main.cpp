#include "etna/etna_core.hpp"
#include "scene.hpp"
#include "scene_graph.hpp"

constexpr uint32_t WINDOW_WIDTH{800};
constexpr uint32_t WINDOW_HEIGHT{600};

using namespace etna;

static Window* g_window{nullptr};

struct UpdateCameraData {
	float sensitivity{0.001f};
	float speed{6.f};
	bool fly{false};
};

void updateCamera(_SceneNode* node, void* _data, float dt, Scene* const) {
	const UpdateCameraData& data = *static_cast<const UpdateCameraData*>(_data);

	Transform transform = node->getTransform();

	transform.yaw += static_cast<float>(g_window->mouseDeltaX()) * data.sensitivity;

	transform.pitch +=
		static_cast<float>(g_window->mouseDeltaY()) * data.sensitivity;

	if (transform.pitch > M_PI / 2) {
		transform.pitch = M_PI / 2;
	} else if (transform.pitch < -M_PI / 2) {
		transform.pitch = -M_PI / 2;
	}

	Vec3& position = transform.position;
	float yaw = -transform.yaw;

	const Vec3 playerForward =
		data.fly ? transform.forward() : Vec3{sinf(yaw), 0, cosf(yaw)} * -1;

	const Vec3 playerRight =
		data.fly ? transform.right() : Vec3{cosf(yaw), 0, -sinf(yaw)};

	const Vec3 up = data.fly ? transform.up() : Vec3{0, 0, 0};

	if (g_window->isKeyPressed(KEY_0)) {
		position = {0, 1, 0};
	}

	if (g_window->isKeyPressed(KEY_W)) {
		position += playerForward * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_S)) {
		position -= playerForward * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_D)) {
		position += playerRight * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_A)) {
		position -= playerRight * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_SPACE)) {
		position += up * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_LEFT_SHIFT)) {
		position -= up * data.speed * dt;
	}

	if (g_window->isKeyPressed(KEY_ESCAPE)) {
		g_window->setCaptureMouse(false);
	}

	if (g_window->isKeyPressed(KEY_ENTER)) {
		g_window->setCaptureMouse(true);
	}

	if (g_window->isKeyPressed(KEY_LEFT_CONTROL)) {
		g_window->setCaptureMouse(true);
	}

	node->updateTransform(transform);
}

struct TestData {
	CameraNode camera{nullptr};
};

void testUpdate(_SceneNode* node, void* _data, float dt, Scene* const) {
	UpdateCameraData& data =
		*static_cast<UpdateCameraData*>(node->getScriptData("CameraScript"));

	if (g_window->isKeyClicked(KEY_F)) {
		data.fly = !data.fly;
	}

	const TestData& testData = *static_cast<TestData*>(_data);

	std::cout << testData.camera->getName() << std::endl;
}

void onCreateTest(_SceneNode* node, void* _data, Scene* const scene) {
	CameraNode camera = scene->getCamera("MainCamera");

	if (camera == nullptr)
		return;

	TestData& testData = *static_cast<TestData*>(_data);

	testData.camera = camera;

	std::cout << "Test script created" << std::endl;
}

int main(void) {
	engine::init();

	g_window = new Window({
		.width = WINDOW_WIDTH,
		.height = WINDOW_HEIGHT,
		.title = "Basic Etna",
		.captureMouse = true,
	});

	Renderer renderer({});

	Scene scene;

	scene.addMesh(scene::createMeshNode({
		.name = "Sphere1",
		.mesh = engine::getSphere(),
		.material = engine::createGridMaterial({
			.color = BLUE * 0.08,
			.gridColor = BLUE,
			.gridSpacing = 0.1,
			.thickness = 0.005,
		}),
		.transform =
			{
				.position = {1.5, 0.5, -5},
				.pitch = M_PI / 2,
				.scale = Vec3(0.5),
			},
	}));

	CameraNode camera = scene.addCamera(scene::createCameraNode({
		.name = "MainCamera",
		.renderTarget = g_window,
	}));

	UpdateCameraData camData{
		.sensitivity = 0.001f,
		.speed = 6.0f,
		.fly = true,
	};

	camera->addScript(new Script({
		.name = "CameraScript",
		.onUpdate = updateCamera,
		.data = &camData,
	}));

	camera->addScript(new Script({
		.name = "TestScript",
		.onUpdate = testUpdate,
		.onCreate = onCreateTest,
		.data = new TestData,
	}));

	scene.setupNodes();

	while (!g_window->shouldClose()) {
		g_window->pollEvents();

		scene.updateNodes();

		scene.render(renderer);

		g_window->swapBuffers();
	}
}
