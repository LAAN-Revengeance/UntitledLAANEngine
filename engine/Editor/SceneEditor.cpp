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
	
	r.Start(true,0.5,0.5,0.0,0.5);
	r.Text("hello world!",0.5);

	r.End();
	//r.Start(false);
	//r.Text("hello world!", 0.2);
	//
	//r.End();
}