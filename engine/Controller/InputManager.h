#pragma once

#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>
#include <Camera.h>
#include <GameObject.h>

/**
*	@Class InputManager
*	@brief allows for custom and dynamic allocation for key input from the game 
*	Setup as a singleton class for easy global access.
* 
*	@author Nathan Choo / Dylan Burgess
*	@version 2.0
*	@date 15/04/2023
*/
typedef void(*actionPTR)();

class InputManager
{
	public:

			/**
			*	@brief returns the current instance of input manager
			*	@return InputManager class
			*/
		static InputManager& Get();

			/**
			*	@brief binds a key to a specific action
			*	@return void
			*/
		void BindKey(std::string action, int newKey);
		
			/**
			*	@brief remove key binded to a specific action
			*	@return void
			*/
		void RemoveKey(std::string action);
		
			/**
			*	@brief remove action from the maps
			*	@return void
			*/
		bool GetKeyState(std::string action);

			/**
			*	@brief binds a function to a specific action
			*	@return void
			*/
		void BindAction(std::string action, actionPTR actionFunc);
		
			/**
			*	@brief remove action from the maps
			*	@return void
			*/
		void RemoveAction(std::string action);

			/**
			*	@brief runs the function binded to each action
			*	@param float deltaTime
			*	@return void
			*/
		void KeyActions(float deltatime);

			/**
			*	@brief the callback function to get the any key change of input from the keyboard
			*	@param GLFWWindow window
			*	@param int key
			*	@param int scancode
			*	@param int action
			*	@param int mods
			*	@return void
			*/
		void GlfwKeyCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods);

			/**
			*	@brief enable the a key for the system to be able to use
			*	@param int key
			*	@return void
			*/
		void EnableKey(int key);

			/**
			*	@brief disable the a key so the system can't us it any more
			*	@param int key
			*	@return void
			*/
		void DisableKey(int key);

			/**
			*	@brief the static callback function to get the any mouse input
			*	@param GLFWWindow window
			*	@param double xpos
			*	@param double ypos
			*	@return static void
			*/
		static void GlfwMouseCallback(GLFWwindow* window, double xpos, double ypos);
		
			/**
			*	@brief the static callback function to get the any keybaord input
			*	@param GLFWWindow window
			*	@param int key
			*	@param int scancode
			*	@param int action
			*	@param int mods
			*	@return static void
			*/
		static void GlfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

			/**
			*	@brief the static callback function to get the scroll wheel input
			*	@param GLFWWindow window
			*	@param double xoffset
			*	@param double yoffset
			*	@return static void
			*/
		static void GlfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		
			/**
			*	@brief the callback function to get the scroll wheel input
			*	@param GLFWWindow window
			*	@param double xoffset
			*	@param double yoffset
			*	@return void
			*/
		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

			/**
			*	@brief the callback function to get the any mouse input
			*	@param GLFWWindow window
			*	@param double xpos
			*	@param double ypos
			*	@return void
			*/
		void mouseCallback(GLFWwindow* window, double xpos, double ypos);

			/**
			*	@brief switchs weather the mouse is visable or not on the screen
			*	@param bool visiable
			*	@return void
			*/
		void SetMouseLock(bool visible);

			/**
			*	@brief get the mouse postion on the screen 
			*	@return glm::vec2 
			*/
		glm::vec2 GetMousePostion();
		
			/**
			*	@brief get the mouse X postion on the screen
			*	@return float
			*/
		float GetMouseX();
		
			/**
			*	@brief get the mouse Y postion on the screen
			*	@return float
			*/
		float GetMouseY();
		
			/**
			*	@brief get the scroll wheel move direction, if moved
			*	@return float
			*/
		float GetScrollOffset();
		
			/**
			*	@brief assigns GLFW keycallback function and assigns window to input manager
			*	@return void
			*/
		void Init(GLFWwindow* window);
		
			/**
			*	@brief gets the current window
			*	@return GLFWwindow*
			*/
		GLFWwindow* GetWindow() { return _Window; };

			/**
			*	@brief get whether the mouse is locked
			*	@return bool mouse lock state
			*/
		bool GetMouseLock();
	private:
			/**
			*	@brief input manager construtor
			*/
		InputManager() :scrollOffset(0){}
		
			/**
			*	@brief copy construtor disable to provent copy since input manager is a singleton
			*/
		InputManager(InputManager const&) = delete;	//prevent copies
		
			/**
			*	@brief = operator disable to provent copy since input manager is a singleton
			*/
		void operator=(InputManager const&) = delete;	//prevent assignments

			/**
			*	stuct to store a key and if it is enable or not
			*/
		struct keyBinding { int key; bool state; };

			/**
			*	Keyinput map which has an aciton binded to a keybinding
			*/
		std::map<std::string, keyBinding> inputMap;
		
			/**
			*	ActionMap map which has an aciton binded to a function
			*/
		std::map<std::string, actionPTR> _ActionMap;
		

			/**
			*	action list to store the avaible actions
			*/
		std::list<std::string> _ActionList;

			/**
			*	window refernce for callback alloaction
			*/
		GLFWwindow* _Window = nullptr;

			/**
			*	the current mouse scroll direction
			*/
		double scrollOffset;
		
			/**
			*	if the mouse has been moved or not
			*/
		bool firstMouse = true;
		
			/**
			*	the last X position of the mouse on the screen
			*/
		double lastX = 0.0f;
		
			/**
			*	the last Y position of the mouse on the screen
			*/
		double lastY = 0.0f;
};