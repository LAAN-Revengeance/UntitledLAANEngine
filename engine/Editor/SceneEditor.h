#pragma once
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Serialization/ProjectLoader.h>
#include "FileExplorer.h"
#include <SoundEngine.h>
#include <Lua/LuaManager.h>


/**
*	@Class SceneEditor
*	@brief Render GUI used for editing a scene
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 16/08/2023
*/

const int LAAN_ENGINE_VERSION = 1;

class SceneEditor
{
public:
	
	static SceneEditor& Get();

	void Run(const char* filePath = "");

	void Draw(double deltaTime);
	void Update(double deltaTime);

	void SaveProject(const char* path);

	void LoadSceneFromFile(const char* path);
	void UseScene(Scene* nscene);

	static void ResizeCallback(GLFWwindow* window, int width, int height);
private:

	//Draw GUI Functions
	void DrawHeighrarchy();
	void DrawInspector();
	void DrawMenu();
	void DrawResources();
	void DrawWindowSettings(bool* showChangeWindow);
	void DrawDebug(bool* showDebugMenu);
	void DrawOpenFile(bool* showOpenFile);
	void DrawSaveFile(bool* showSaveFile);
	void Draw3DWidget();
	void CameraControl(double deltaTime);
	void CheckKeys();

	//File Save Functions
	std::string FilterFilePath(std::string filePath);

	//reference to glfw window
	GLFWwindow* window = nullptr;
	
	//mouse controls
	float lastX = 0;
	float lastY = 0;
	float mouseSensitivity = 10.0f;

	//timestep
	bool isRunning = false;
	double deltaTime = 0.0;
	double previousFrameTime = 0.0;

	//save file path
	std::string saveFilePath = "";
	std::string luaFilePath = "resources/scripts/main.lua";
	std::string windowName = "editor";

	Scene* scene = nullptr;
	Camera camera;
	GameObject* inspectedObject = nullptr;
	GameObject* lastObject = nullptr;

	SceneEditor();
	~SceneEditor();
	SceneEditor(const SceneEditor&) = delete;
	SceneEditor& operator = (const SceneEditor&) = delete;

	bool isFreecam = true;
	bool isPhysicDebug = true;

	//references to managers
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


	friend class GameEngine;
};
