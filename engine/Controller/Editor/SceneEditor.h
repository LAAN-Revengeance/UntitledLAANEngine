#pragma once
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Serialization/ProjectLoader.h>
#include "FileExplorer.h"
#include <SoundEngine.h>
#include <GaemEngine.h>
#include <lua/LuaGameBridge.h>


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
	SceneEditor(GameEngine* nEngine);
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
	void DrawDebug(bool* showDebugMenu);
	void DrawOpenFile(bool* showOpenFile);
	void DrawSaveFile(bool* showSaveFile);
	void Draw3DWidget();

	//Input methods
	void CameraControl(double deltaTime);
	void CheckKeys();

	void SetLuaFile(std::string nluaFile);

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

		///Main GUI Renderer
	GUIRenderer& guirenderer;
	GaemGizmo::Line pathDebugLine;
	GaemGizmo::Box selectedNavNodeBox;

};
