#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>
#include <cmath> 
#include <Utils/GaemUtils.h>
#include <time.h>

GameEngine::GameEngine(Window* nWindow, GaemEvents::EventDispatcher* nDispatcher, Timer* nTimer):
	timer(nTimer),
	renderer(nWindow),
	isRunning(true),
	scene(new Scene),
	window(nWindow),
	eventDispatcher(nDispatcher),
	aiManager(nullptr),
	msgDispatcher(nullptr)
{
	InputManager::Get().Init(nWindow);
	guiRenderer.Init(nWindow);

	//bind renderer resize handler to windowResize event
	eventDispatcher->Subscribe("windowResize"	, std::bind(&Renderer::HandleResizeEvent,&renderer, std::placeholders::_1));
	eventDispatcher->Subscribe("InputPoll", std::bind(&InputManager::PollEventsCallback, &InputManager::Get(), std::placeholders::_1));
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
	guiRenderer.StartGUI();

	scene->UpdateFunction.Execute(deltaTime);
	scene->physicsWorld.Update(deltaTime);

	for (auto& it : scene->gameObjects) {
		it.second->Update(deltaTime);
	}

	if (aiManager)
		aiManager->UpdateAgents(deltaTime);

	if (msgDispatcher)
		msgDispatcher->SendMsgQueue();

	guiRenderer.EndGUI();
}

void GameEngine::Draw(double deltaTime)
{
	if (!scene)
		return;
	renderer.RenderScene(scene->camera, *scene, deltaTime);
}


