#pragma once

#include <Renderer.h>
#include <GUIRenderer.h>
#include <InputManager.h>
#include <LayerStack.h>
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
	GameEngine();
	~GameEngine();
	void Run();

	void PushLayer(Layer* layer);
	Layer* PopLayer();

private:
	
	Window window;

	LayerStack layers;

	Timer timer;
	
	Renderer renderer;

	GameEngine& operator =(const GameEngine&) = delete;
	GameEngine(const GameEngine&) = delete;
};