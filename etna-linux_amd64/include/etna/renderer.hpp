#pragma once

#include <cstdint>
#include "ignis/command.hpp"
#include "ignis/fence.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "render_target.hpp"
#include "color.hpp"

namespace etna {

struct RenderFrameSettings {
	Color clearColor{0.02f, 0.02f, 0.02f, 1};
	VkAttachmentStoreOp colorStoreOp{VK_ATTACHMENT_STORE_OP_STORE};
	VkAttachmentStoreOp depthStoreOp{VK_ATTACHMENT_STORE_OP_DONT_CARE};
	VkAttachmentLoadOp colorLoadOp{VK_ATTACHMENT_LOAD_OP_CLEAR};
	VkAttachmentLoadOp depthLoadOp{VK_ATTACHMENT_LOAD_OP_CLEAR};
	bool renderDepth{true};
};

struct Viewport {
	float x{0};
	float y{0};
	float width{0};
	float height{0};
};

struct DrawSettings {
	MeshHandle mesh{nullptr};
	MaterialHandle material{nullptr};
	Mat4 transform{};
	Viewport viewport;
	ignis::BufferId buff1{IGNIS_INVALID_BUFFER_ID};
	ignis::BufferId buff2{IGNIS_INVALID_BUFFER_ID};
	ignis::BufferId buff3{IGNIS_INVALID_BUFFER_ID};
	ignis::BufferId instanceBuffer{IGNIS_INVALID_BUFFER_ID};
	uint32_t instanceCount{1};
};

constexpr RenderFrameSettings LOAD_PREVIOUS{
	.colorLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD,
};

class Renderer {
public:
	struct CreateInfo {
		uint32_t framesInFlight{2};
	};

	Renderer(const CreateInfo&);

	~Renderer();

	void beginFrame(const RenderTarget&, const RenderFrameSettings& = {});
	void endFrame();

	void draw(const DrawSettings& = {});

	void clearViewport(Viewport viewport, Color color = {});

	ignis::Command& getCommand() const { return *m_frames[m_currentFrame].cmd; }

	const RenderTarget& getRenderTarget() const { return *m_currTarget; }

private:
	struct FrameData {
		ignis::Fence* inFlight;
		ignis::Command* cmd;
	};

	const RenderTarget* m_currTarget{nullptr};

	std::vector<FrameData> m_frames;

	uint32_t m_framesInFlight;
	uint32_t m_currentFrame{0};

public:
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;
};

}  // namespace etna
