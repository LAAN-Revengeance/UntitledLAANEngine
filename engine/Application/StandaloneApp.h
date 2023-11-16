#pragma once
#pragma once
#include <Window.h>
#include <Timer.h>
#include <GaemEngine.h>

class GaemApp
{
public:
	GaemApp(const char* path);
	~GaemApp();
	void Run();

private:

	GaemEvents::EventDispatcher eventDispatcher;
	Window window;
	GameEngine game;
	Timer timer;
	DebugLogger logger;
};
