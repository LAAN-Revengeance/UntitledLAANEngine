#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>

GameEngine::GameEngine(Window* nWindow, EventDispatcher* nDispatcher):
	renderer(nWindow),
	isRunning(true),
	scene(new Scene),
	window(nWindow),
	eventDispatcher(nDispatcher)
{
	InputManager::Get().SetActiveWindow(window);

	eventDispatcher->Subscribe("windowResize", std::bind(&Renderer::HandleResizeEvent,renderer, std::placeholders::_1));
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


