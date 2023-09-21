#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine::GameEngine(Window* window):
	renderer(window)
{
}

GameEngine::~GameEngine() {

}

void GameEngine::Init()
{

}

void GameEngine::Update(double deltaTime) 
{
	if (!scene)
		return;
	for (auto& it : scene->gameObjects) {
		it.second->Update(deltaTime);
	}
	scene->physicsWorld.Update(deltaTime);
}

void GameEngine::Draw(double deltaTime)
{
	if (!scene)
		return;
	renderer.RenderScene(scene->camera, *scene, deltaTime);
}


