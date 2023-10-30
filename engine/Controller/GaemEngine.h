#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Timer.h>
#include <Window.h>

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
	GameEngine(Window*, GaemEvents::EventDispatcher*);
	~GameEngine();
	void Init();
	void Update(double deltaTime);
	void Draw(double deltaTime);

	Renderer renderer;
	GUIRenderer guiRenderer;

private:
	bool isRunning;
	Scene* scene;
	Window* window;
	GaemEvents::EventDispatcher* eventDispatcher;

	friend class SceneEditor;
	friend class LuaGameBridge;
};