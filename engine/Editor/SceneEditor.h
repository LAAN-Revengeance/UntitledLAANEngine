#pragma once
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Scene.h>

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

	static SceneEditor& Get();

	void Draw();
	void Update(double deltaTime);

	void UseScene(Scene* nscene);


	
private:

	void DrawHeighrarchy();
	void DrawInspector();
	void DrawMenu();
	void DrawWindowSettings(bool* showChangeWindow);
	void DrawDebug(bool* showDebugMenu);

	void CameraControl(double deltaTime);


	float lastX = 0;
	float lastY = 0;
	float mouseSensitivity = 10.0f;

	Scene* scene = nullptr;
	Camera camera;
	GameObject* inspectedObject = nullptr;

	GUIRenderer& r = GUIRenderer::Get();

	SceneEditor();
	~SceneEditor();
	SceneEditor(const SceneEditor&) = delete;
	SceneEditor& operator = (const SceneEditor&) = delete;

	friend class GameEngine;
};
