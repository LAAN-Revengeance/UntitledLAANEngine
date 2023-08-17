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

void SceneEditor::UseScene(Scene* nscene)
{
	if (nscene) {
		scene = nscene;
	}
}

void SceneEditor::DrawInspector()
{
	
	r.Start(true,0.2,1,0.0,0.0);
	
	for(auto& pair : scene->gameObjects)
	{
		r.Button(pair.second->name,0.5,100,20);

	}
	r.Text("hello world!",0.5);

	r.End();
	//r.Start(false);
	//r.Text("hello world!", 0.2);
	//
	//r.End();
}