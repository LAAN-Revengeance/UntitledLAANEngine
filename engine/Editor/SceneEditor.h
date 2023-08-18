#pragma once
#include <GUIRenderer.h>
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

	void UseScene(Scene* nscene);

private:

	void DrawHeighrarchy();
	void DrawInspector();
	
	Scene* scene = nullptr;
	GameObject* inspectedObject = nullptr;

	GUIRenderer& r = GUIRenderer::Get();

	SceneEditor();
	~SceneEditor();
	SceneEditor(const SceneEditor&) = delete;
	SceneEditor& operator = (const SceneEditor&) = delete;
};
