#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <AI/AIManager.h>
#include <SoundEngine.h>
#include <Lua/LuaManager.h>
#include <Serialization/SceneLoader.h>
#include <AI/States/ScriptableState.h>
#include <Physics/PhysicsManager.h>

const int WINDOW_TYPE_WINDOWED	 = 1;
const int WINDOW_TYPE_BORDERLESS = 2;
const int WINDOW_TYPE_FULLSCREEN = 3;

/**
*	@Class GameEngine
*	@brief Used to manage the engine, windowing, controllers and run the main loop.
*	Setup as a singleton class for easy global access.
* 
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class GameEngine
{
public:

		/**
		*	@brief Get the singleton instance of GameEngine
		*	@return The singleton instance of GameEngine
		*/
	static GameEngine& Get();

		/**
		*	@brief Runs the main game loop
		*	@return void
		*/
	void Run();

		/**
		*	@brief returns current game time
		*	@return time since start of game
		*/
	double Time();

		/**
		*	@brief Calculates and returns time since last frame
		*	@return time since last frame
		*/
	double DeltaTime();

		/**
		*	@brief GLFW callback for window resizing. recalulates camera properties and viewport
		*	@param width new window width
		*	@param height new window height
		*	@return void
		*/
	static void ResizeCallback(GLFWwindow* window, int width, int height);	

		/**
		*	@brief destroys context windows and frees up recources
		*	@return void
		*/
	void Shutdown();

		/**
		*	@brief Set if the AI and physics should be updated
		*	@param isRun if simulation should be running
		*	@return void
		*/
	void SetSimulation(bool isRun);

		/**
		*	@brief returns is simulation is running
		*	@return bool if sim is running
		*/
	bool IsSimRunning();

		/**
		*	@brief Change the active scene being rendered and simulated
		*	@param Scene new scene to be used
		*	@return void
		*/
	void SwitchScenes(Scene& nscene);

		/**
		*	@brief Change the active windows type; such as borderless, fullscreen etc.
		*	@param type type of window to be set to. Window types are:
		*	<ul><li>WINDOW_TYPE_WINDOWED</li>
		*	<li>WINDOW_TYPE_BORDERLESS</li>
		*	<li>WINDOW_TYPE_FULLSCREEN</li></ul>
		* 
		*	@return void
		*/
	void SetWindowType(int type);

		/**
		*	@brief Set the window icon
		*	@param path filepath to icon
		*	@return void
		*/
	void SetWindowIcon(std::string path);

		/**
		*	@brief Set the window title
		*	@param new window name
		*	@return void
		*/
	void SetWindowName(std::string path);

		///Window width
	int wWidth = 1920;
		///Window height
	int wHeight = 1080;
		///Contains all data for rendering scene and stores world physics info
	Scene* scene;
		///Window being drawn to and being used for inputs
	GLFWwindow* window;
		
		///Main Renderer
	Renderer& renderer = Renderer::Get();
		///Main GUI Renderer
	GUIRenderer& guirenderer = GUIRenderer::Get();
		///Main Lua Manager
	LuaManager& luaManager = LuaManager::Get();
		///Reference to input manager
	InputManager& inputMngr = InputManager::Get();
		///Reference to AI manager
	AIManager& aiManager = AIManager::Get();
		///Reference to SoundEngine
	SoundEngine& soundEngine = SoundEngine::Get();
		//Reference physics Manager
	PhysicsManager& physicsManager = PhysicsManager::Get();

private:

		//expose game engine windowing and special functions to lua
	void ExposeToLua();
		///previous time between frames
	double previousFrameTime = 0.0f;
		///time since last frame
	double deltaTime = 0.0f;
		///whether AI and physics are being updated
	bool simIsRunning = true;
		///Default constructor. Private becuase singleton
	GameEngine();
		///Default Destructor.
	~GameEngine();
		///Assignment operator. Private becuase singleton
	GameEngine& operator =(const GameEngine&) = delete;
		///Copy constructor. Private becuase singleton
	GameEngine(const GameEngine&) = delete;
};