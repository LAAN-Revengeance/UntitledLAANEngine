#pragma once
#include <Window.h>
#include <Timer.h>
#include <GaemEngine.h>
#include <Editor/SceneEditor.h>

class Application
{
public:
	Application();
	~Application();
	void Run();

private:

	GaemEvents::EventDispatcher eventDispatcher;
	Window window;
	GameEngine game;
	SceneEditor editor;
	Timer timer;
	DebugLogger logger;
};
