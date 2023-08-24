#pragma once
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Serialization/SceneLoader.h>

/**
*	@Class SceneEditor
*	@brief Render GUI used for editing a scene
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 16/08/2023
*/

static int LAAN_ENGINE_VERSION = 1;

class SceneEditor
{
public:
	
	static SceneEditor& Get();

	void Run(const char* filePath = "");

	void Draw(double deltaTime);
	void Update(double deltaTime);

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

	//renderer
	GLFWwindow* window = nullptr;
	Renderer& renderer = Renderer::Get();

	//mouse controls
	float lastX = 0;
	float lastY = 0;
	float mouseSensitivity = 10.0f;

	//timestep
	double deltaTime = 0.0;
	double previousFrameTime = 0.0;

	//save file path
	char saveFilePath[256] = "";

	Scene* scene = nullptr;
	Camera camera;
	GameObject* inspectedObject = nullptr;
	GameObject* lastObject = nullptr;

	GUIRenderer& r = GUIRenderer::Get();

	SceneEditor();
	~SceneEditor();
	SceneEditor(const SceneEditor&) = delete;
	SceneEditor& operator = (const SceneEditor&) = delete;

	friend class GameEngine;
};
