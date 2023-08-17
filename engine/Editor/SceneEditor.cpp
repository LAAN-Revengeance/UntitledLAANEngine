#include "SceneEditor.h"



SceneEditor::SceneEditor()
{
}

SceneEditor::~SceneEditor()
{
}

SceneEditor& SceneEditor::Get()
{
	static SceneEditor s_instance;
	return s_instance;
}

void SceneEditor::Draw()
{
	DrawInspector();


}

void SceneEditor::DrawInspector()
{
	r.Start(false);
	r.Text("hello world!",0.5);

	r.End();
}