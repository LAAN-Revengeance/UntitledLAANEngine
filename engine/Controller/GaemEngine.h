#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Timer.h>
#include <Window.h>
#include <AIManager.h>

/**
*	@Class GameEngine
*	@brief Runs the main game loop.
* 
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 15/08/2023
*/
class GameEngine
{
public:
	GameEngine(Window*, GaemEvents::EventDispatcher*, Timer* nTimer);
	~GameEngine();
	void Init();
	void Update(double deltaTime);
	void Draw(double deltaTime);

	Renderer renderer;
	GUIRenderer guiRenderer;

private:
	bool isRunning;

	Window* window;
	Timer* timer;
	GaemEvents::EventDispatcher* eventDispatcher;

	Scene* scene;
	AIManager* aiManager;
	Dispatcher* msgDispatcher;

	friend class SceneEditor;
	friend class LuaGameBridge;
};