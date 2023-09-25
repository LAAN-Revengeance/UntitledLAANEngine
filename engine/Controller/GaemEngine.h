#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Timer.h>
#include <Window.h>
#include <Event/EventDispatcher.h>

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
	GameEngine(Window*,EventDispatcher*);
	~GameEngine();
	void Init();
	void Update(double deltaTime);
	void Draw(double deltaTime);

private:
	bool isRunning;
	Scene* scene;
	Renderer renderer;
	GUIRenderer guiRenderer;
	Window* window;
	EventDispatcher* eventDispatcher;

	friend class SceneEditor;
	friend class LuaGameBridge;
};