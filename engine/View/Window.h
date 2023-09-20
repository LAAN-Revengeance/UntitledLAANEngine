#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "Graphics/Graphics.h"

typedef void(*WindowResizeFunc)(int width, int height);

enum WindowType {
	WINDOW_TYPE_WINDOWED,
	WINDOW_TYPE_BORDERLESS,
	WINDOW_TYPE_FULLSCREEN
};

/**
*	@Class Window
*	@brief Used to manage a window and its rendering context.
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 18/09/2023
*/
class Window
{
public:

	Window(int w, int h,const std::string& wName);
	~Window();

	bool IsWindowClosed();
	void CloseWindow();

	void SwapBuffers();
	void PollEvents();

	void SetResizeCallback(WindowResizeFunc callback);

	void MakeCurrentContext();

	int GetWidth();
	int GetHeight();

	void SetName(std::string nName);
	void SetIcon(std::string path);
	void SetWindowType(WindowType type);

	void SetMouseLock(bool isLocked);

private:
	static void _mGlFWCallback(GLFWwindow* window, int width, int height);
	WindowResizeFunc* resizeCallback = nullptr;

	static bool _glfwInit;

	GLFWwindow* window;
	std::string name;

	static std::vector<GLFWwindow*> activeWindows;

	friend class InputManager;
};

