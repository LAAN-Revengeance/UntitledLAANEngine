#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <Timer.h>
#include <Window.h>
#include <Debug/DebugDrawables.h>

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

private:
	bool isRunning;
	Scene* scene;
	Renderer renderer;
	GUIRenderer guiRenderer;
	Window* window;
	GaemEvents::EventDispatcher* eventDispatcher;

	GaemGizmo::Line testLine;

	friend class SceneEditor;
	friend class LuaGameBridge;
};