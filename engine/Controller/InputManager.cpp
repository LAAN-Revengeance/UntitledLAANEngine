#include "InputManager.h"

InputManager& InputManager::Get()
{
	static InputManager instance;
	return instance;
}

void InputManager::BindKey(std::string action, int newKey)
{
	inputMap[action] = keyBinding{ newKey, false };
}

void InputManager::RemoveKey(std::string action)
{
	inputMap[action] = keyBinding{ -1, false };
}

bool InputManager::GetKeyState(std::string action)
{
	std::map<std::string, keyBinding>::iterator it = inputMap.find(action);
	if (it != inputMap.end())
	{
		return it->second.state;
	}
	return false;
}

void InputManager::BindAction(std::string action, actionPTR actionFunc)
{
	_ActionMap[action] = actionFunc;
}

void InputManager::RemoveAction(std::string action)
{
	_ActionList.remove(action);
	inputMap.erase(action);
	_ActionMap.erase(action);
}

void InputManager::KeyActions(float deltatime)
{
	for (auto action : _ActionList)
	{
		for (auto key : inputMap)
		{
			keyBinding bind = key.second;
			if (bind.state == true)
			{
				if (key.first == action)
					_ActionMap[action]();
			}
		}
	}
}

void InputManager::mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	double xoffset = xPos - lastX;
	double yoffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;
}

void InputManager::GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	Get().ScrollCallback(window, xoffset, yoffset);
}

void InputManager::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scrollOffset = yoffset;
}

void InputManager::SetMouseLock(bool isLock)
{
	if (!_Window)
		return;
	if(isLock)
	{
		glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	else
	{
		glfwSetInputMode(_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

glm::vec2 InputManager::GetMousePostion()
{
	if (_Window) {
		double xPos, yPos;
		glfwGetCursorPos(_Window, &xPos, &yPos);
		return glm::vec2{ xPos, yPos };
	}
	return glm::vec2(0,0);
}

float InputManager::GetMouseX() { 
	return GetMousePostion().x;
}

float InputManager::GetMouseY() {
	return GetMousePostion().y;
}

float InputManager::GetScrollOffset()
{
	double yOffset = scrollOffset;
	scrollOffset = 0;
	return yOffset;
}

void InputManager::GlfwKeyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS) 
		EnableKey(key);
	else if (action == GLFW_RELEASE)
		DisableKey(key);
}

void InputManager::EnableKey(int key) 
{
	for (auto it : inputMap)
	{
		keyBinding& bind = inputMap[it.first]; 
		if (bind.key == key)
			bind.state = true; 
	}
}

void InputManager::DisableKey(int key)
{
	for (auto it : inputMap)
	{
		keyBinding& bind = inputMap[it.first];
		if (bind.key == key)
			bind.state = false;
	}
}

void InputManager::GlfwMouseCallback(GLFWwindow* window, double xpos, double ypos)
{
		Get().mouseCallback(window, xpos, ypos);
}

void InputManager::GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Get().GlfwKeyCallbackDispatch(window, key, scancode, action, mods);
}

void InputManager::Init(GLFWwindow* window) {
	_Window = window;
	glfwSetKeyCallback(window, GlfwKeyCallback);
	glfwSetCursorPosCallback(window, GlfwMouseCallback);
	glfwSetKeyCallback(window, GlfwKeyCallback);
	glfwSetScrollCallback(window, GlfwScrollCallback);
}

bool InputManager::GetMouseLock()
{
	if (glfwGetInputMode(_Window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return true;
	
	return false;
}
