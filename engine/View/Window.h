#pragma once
#include <GLFW/glfw3.h>
#include <string>
#include "Graphics/Graphics.h"
#include <Event/EventDispatcher.h>

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

	Window(int w, int h,const std::string&, GaemEvents::EventDispatcher*);
	~Window();

	bool IsWindowClosed();
	void CloseWindow();

	void SwapBuffers();
	void PollEvents();

	void MakeCurrentContext();

	int GetWidth();
	int GetHeight();

	void SetName(std::string nName);
	void SetIcon(std::string path);
	void SetWindowType(WindowType type);

	void SetMouseLock(bool isLocked);

	//return currently active window
	static Window* GetActiveWindow();
private:
	static void _mGlFWCallback(GLFWwindow* window, int width, int height);

	static bool _glfwInit;
	static Window* currentWindow;
	GLFWwindow* window;
	std::string name;

	static std::vector<GLFWwindow*> activeWindows;

	GaemEvents::EventDispatcher* eventDispatcher;

	friend class InputManager;
	friend class GUIRenderer;
};

