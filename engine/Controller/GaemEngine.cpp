#include "GaemEngine.h"
#include <string.h>
#include <iostream>

GameEngine::GameEngine(Window* nWindow):
	renderer(nWindow),
	isRunning(true),
	scene(new Scene),
	window(nWindow)
{
	InputManager::Get().SetActiveWindow(window);
}

GameEngine::~GameEngine() {

}

void GameEngine::Init()
{
	scene->InitFunction.Execute();
}

void GameEngine::Update(double deltaTime) 
{
	if (!scene)
		return;
	for (auto& it : scene->gameObjects) {
		it.second->Update(deltaTime);
	}
	scene->UpdateFunction.Execute(deltaTime);
	scene->physicsWorld.Update(deltaTime);

}

void GameEngine::Draw(double deltaTime)
{
	if (!scene)
		return;
	renderer.RenderScene(scene->camera, *scene, deltaTime);
}


