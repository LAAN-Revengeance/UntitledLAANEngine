#pragma once
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Serialization/ProjectLoader.h>
#include "FileExplorer.h"
#include <SoundEngine.h>
#include <GaemEngine.h>
#include <lua/LuaGameBridge.h>
#include <Utils/DebugLogger.h>


const int LAAN_ENGINE_VERSION = 2;

/**
*	@Class SceneEditor
*	@brief Render GUI used for editing a scene
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 16/08/2023
*/
class SceneEditor
{
public:
	SceneEditor(GameEngine* nEngine, DebugLogger* logger);
	~SceneEditor();
	void Update(double deltaTime);
	void Draw(double deltaTime);
	void SaveProject(const char* path);
	void LoadSceneFromFile(const char* path);
	void UseScene(Scene* nscene);
private:

	//Draw GUI Functions
	void DrawHeighrarchy();
	void DrawInspector();
	void DrawMenu();
	void DrawResources();
	void DrawWindowSettings(bool* showChangeWindow);
	void DrawOpenFile(bool* showOpenFile);
	void DrawSaveFile(bool* showSaveFile);
	void Draw3DWidget();
	void DrawDebug();
	void DrawNPCInspector();

	//Input methods
	void CameraControl(double deltaTime);
	void CheckKeys();

	//change the current scenes lua file
	void SetLuaFile(std::string nluaFile);

	//pointer to engine this editor is inerracting with
	GameEngine* engine;

	//File Save Functions
	std::string FilterFilePath(std::string filePath);

	//mouse controls
	float lastX = 0;
	float lastY = 0;
	float mouseSensitivity = 10.0f;

	//save file path
	std::string saveFilePath = "";
	std::string luaFilePath = "";
	std::string windowName = "editor";

	Camera* camera;

	GameObject* inspectedObject = nullptr;
	GameObject* lastObject = nullptr;

	bool isFreecam = false;
	bool isPhysicDebug = true;
	bool isPathDebug = true;
	bool isShowWidget = true;
	bool isGUIOnRun = true;

	///Main GUI Renderer
	GUIRenderer& guirenderer;
	GaemGizmo::Line pathDebugLine;
	GaemGizmo::Box selectedNavNodeBox;

	///Debug logger
	DebugLogger* _logger;
};
