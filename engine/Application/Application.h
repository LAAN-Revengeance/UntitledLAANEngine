#pragma once
#include <Window.h>
#include <Timer.h>
#include <GaemEngine.h>

class Application
{
public:
	Application();
	~Application();
	void Run();

private:

	Window window;
	GameEngine game;
	Timer timer;
};
