#include "InputManager.h"

void InputManager::SetActiveWindow(Window* nWindow)
{
	if(nWindow)
		_Window = nWindow;
}

InputManager& InputManager::Get() {
	static InputManager i_Instance;
	return i_Instance;
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

void InputManager::KeyActions(double deltatime)
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
	_Window->SetMouseLock(isLock);
}

glm::vec2 InputManager::GetMousePostion()
{
	if (_Window) {
		double xPos, yPos;
		glfwGetCursorPos(_Window->window, &xPos, &yPos);
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
	float yOffset = scrollOffset;
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
	auto& input = Get();

	//run key callbacks
	input.GlfwKeyCallbackDispatch(window, key, scancode, action, mods);

	//store key states
	if (key >= 0 && key <= GLFW_KEY_LAST) {
		if (action == GLFW_PRESS) {
			if (!input.keyStates[key].pressed) {
				input.keyStates[key].justPressed = true;
			}
		}
		else if (action == GLFW_RELEASE) {
			input.keyStates[key].pressed = false;
			input.keyStates[key].justPressed = false;
		}
	}
}

void InputManager::Init(Window* window) {
	_Window = window;
	glfwSetKeyCallback(_Window->window, GlfwKeyCallback);
	glfwSetScrollCallback(_Window->window, GlfwScrollCallback);
}

bool InputManager::GetMouseLock()
{
	if (!_Window)
		return false;
	if (glfwGetInputMode(_Window->window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		return true;
	
	return false;
}

bool InputManager::GetKeyPressed(unsigned int key)
{
	if (!_Window)
		return false;
	return glfwGetKey(_Window->window, key);
}

bool InputManager::GetKeyPressedDown(unsigned int key)
{
	if (!_Window)
		return false;

	if (key >= 0 && key <= GLFW_KEY_LAST) {
		return keyStates[key].justPressed;
	}
	return false;
}

void InputManager::PollEventsCallback(const GaemEvents::Event& event)
{
	//Reset justPressed state
	for (int key = 0; key <= GLFW_KEY_LAST; ++key) {
		Get().keyStates[key].justPressed = false;
	}
}
