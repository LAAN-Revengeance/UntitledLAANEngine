#include "Window.h"

bool Window::_glfwInit = false;

Window::Window(int w, int h, const std::string& wName)
{
	if (!_glfwInit) {
		glfwInit();
		_glfwInit = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(w, h, wName.c_str(), NULL, NULL);

	if (!window)
	{
		std::cout << "ERROR Could not initalize window." << std::endl;
		glfwTerminate();
		return;
	}

}

Window::~Window()
{
}

bool Window::IsWindowClosed()
{
	if(window)
		return glfwWindowShouldClose(window);

	return false;
}

void Window::CloseWindow()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(window);
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::SetResizeCallback(WindowResizeFunc callback)
{
	//glfwSetFramebufferSizeCallback(window, (*resizeCallback));
}

void Window::MakeCurrentContext()
{
	glfwMakeContextCurrent(window);
}

int Window::GetWidth()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return width;
}

int Window::GetHeight()
{
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	return height;
}

void Window::SetName(std::string nName)
{
	name = nName;
	glfwSetWindowTitle(window, name.c_str());
}

void Window::SetIcon(std::string path)
{
	Texture tex(path.c_str());
	GLFWimage image;
	image.pixels = tex.GetImageData();
	image.width = tex.GetWidth();
	image.height = tex.GetHeight();
	glfwSetWindowIcon(window, 1, &image);
}

void Window::SetWindowType(WindowType type)
{
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();;
	const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);;
	switch (type) {
	case WINDOW_TYPE_WINDOWED: // Windowed Window
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_TRUE);
		glfwSetWindowMonitor(window, nullptr, 1, 1, videoMode->width, videoMode->height, 0);
		break;
	case WINDOW_TYPE_BORDERLESS: // Borderless Window
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowAttrib(window, GLFW_RESIZABLE, GLFW_FALSE);
		glfwSetWindowMonitor(window, nullptr, 0, 0, videoMode->width, videoMode->height, 0);
		break;
	case WINDOW_TYPE_FULLSCREEN: // Fullscreen Window
		glfwSetWindowMonitor(window, primaryMonitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
		break;
	}
}

void Window::SetMouseLock(bool isLocked)
{
	if (!window)
		return;
	if (!isLocked)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

void Window::_mGlFWCallback(GLFWwindow* window, int width, int height)
{

	
}
