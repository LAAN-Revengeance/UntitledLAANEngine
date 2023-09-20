#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine::GameEngine()
{
	
}

GameEngine::~GameEngine() {

}

void GameEngine::Run() {

	timer.Reset();

	while (true)
	{
		double deltaTime;
		for (auto& layer : layers)
			layer->OnUpdate(deltaTime);

		for (auto& layer : layers)
			layer->OnDraw(deltaTime);
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

