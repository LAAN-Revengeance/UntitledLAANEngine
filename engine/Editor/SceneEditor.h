#pragma once
#include <GUIRenderer.h>

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

private:


	void DrawInspector();
	SceneEditor();
	~SceneEditor();
	SceneEditor(const SceneEditor&) = delete;
	SceneEditor& operator = (const SceneEditor&) = delete;

	GUIRenderer& r = GUIRenderer::Get();
};
