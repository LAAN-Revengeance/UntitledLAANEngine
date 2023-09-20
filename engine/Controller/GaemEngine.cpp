#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine::GameEngine() : 
	window(Window(500, 500, "engine")),
	renderer(&window)
{
	
}

GameEngine::~GameEngine() {

}

void GameEngine::Run() {

	timer.Reset();

	while (!window.IsWindowClosed())
	{
		timer.Update();
		window.PollEvents();

		double deltaTime = timer.DeltaTime();
		for (auto& layer : layers)
			layer->OnUpdate(deltaTime);

		for (auto& layer : layers)
			layer->OnDraw(deltaTime);

		window.SwapBuffers();
	}
}

void GameEngine::PushLayer(Layer* layer)
{
	layers.Push(layer);
}

Layer* GameEngine::PopLayer()
{
	return layers.Pop();
}

