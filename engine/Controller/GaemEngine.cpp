#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>
#include <cmath> 
#include <Utils/GaemUtils.h>
#include <time.h>

GameEngine::GameEngine(Window* nWindow, GaemEvents::EventDispatcher* nDispatcher):
	renderer(nWindow),
	isRunning(true),
	scene(new Scene),
	window(nWindow),
	eventDispatcher(nDispatcher)
{
	InputManager::Get().Init(nWindow);
	guiRenderer.Init(nWindow);

	//bind renderer resize handler to windowResize event
	eventDispatcher->Subscribe("windowResize", std::bind(&Renderer::HandleResizeEvent,&renderer, std::placeholders::_1));
}

GameEngine::~GameEngine() {

}

void GameEngine::Init()
{
	scene->InitFunction.Execute();
}

void GameEngine::Update(double deltaTime) 
{
	if (!scene || !isRunning)
		return;

	scene->UpdateFunction.Execute(deltaTime);
	scene->physicsWorld.Update(deltaTime);

	for (auto& it : scene->gameObjects) {
		it.second->Update(deltaTime);
	}
}

void GameEngine::Draw(double deltaTime)
{
	if (!scene)
		return;
	renderer.RenderScene(scene->camera, *scene, deltaTime);

}


