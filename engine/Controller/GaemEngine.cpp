#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine::GameEngine(Window* window):
	renderer(window)
{
	//InputManager::Get().Init(&window);
}

GameEngine::~GameEngine() {

}

void GameEngine::Init()
{

}

void GameEngine::Update(double deltaTime) {
	
	

}

void GameEngine::Draw(double deltaTime)
{
	//renderer.RenderScene();
}


