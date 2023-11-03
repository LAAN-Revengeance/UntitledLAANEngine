#pragma once
#include <Window.h>
#include <Timer.h>
#include <GaemEngine.h>
#include <Editor/SceneEditor.h>

class EditorApp
{
public:
	EditorApp();
	~EditorApp();
	void Run();

private:

	GaemEvents::EventDispatcher eventDispatcher;
	Window window;
	GameEngine game;
	SceneEditor editor;
	Timer timer;
	DebugLogger logger;
};
