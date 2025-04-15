#pragma once

#include <unordered_map>
#include "ignis/swapchain.hpp"
#include "render_target.hpp"
#include "key_map.hpp"

struct GLFWwindow;

namespace etna {

class Window : public RenderTarget {
public:
	struct CreateInfo {
		uint32_t width{0};
		uint32_t height{0};
		const char* title{"Etna Window"};
		bool captureMouse{false};
	};

	Window(const CreateInfo&);

	~Window();

	void pollEvents();
	void swapBuffers();

	bool shouldClose() const;
	bool isKeyPressed(Key) const;
	bool isKeyClicked(Key);
	bool isMouseCaptured() const;

	double getMouseX() const;
	double getMouseY() const;

	double mouseDeltaX() const;
	double mouseDeltaY() const;

	void setCaptureMouse(bool capture);

private:
	ignis::Swapchain* m_swapchain;
	GLFWwindow* m_window;
	VkSurfaceKHR m_surface{VK_NULL_HANDLE};
	CreateInfo m_creationInfo;

	double m_lastMouseX{0};
	double m_lastMouseY{0};
	double m_mouseDeltaX{0};
	double m_mouseDeltaY{0};

	std::unordered_map<int, bool> m_prevKeyStates;

	ignis::Command* m_blitCmd;
	ignis::Semaphore* m_imageAvailable;
	ignis::Semaphore* m_finishedBlitting;

public:
	Window(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(const Window&) = delete;
	Window& operator=(Window&&) = delete;
};

}  // namespace etna
