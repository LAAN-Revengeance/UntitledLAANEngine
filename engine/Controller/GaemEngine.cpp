#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>

GameEngine::GameEngine(Window* nWindow, GaemEvents::EventDispatcher* nDispatcher):
	renderer(nWindow),
	isRunning(true),
	scene(new Scene),
	window(nWindow),
	eventDispatcher(nDispatcher),
	guiRenderer(nWindow)
{
	InputManager::Get().Init(nWindow);

	//bind renderer resize handler to windowResize event
	eventDispatcher->Subscribe("windowResize", std::bind(&Renderer::HandleResizeEvent,&renderer, std::placeholders::_1));

	std::vector<glm::vec3> positions;
	for (int i = 0; i < 10; i++)
	{
		glm::vec3 nVec(i * 2.0f, 0.0f, i * 0.5f);
		positions.push_back(nVec);
	}
	testLine.SetLine(positions);
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

	//testLine.Render(scene->camera.GetProjection(),scene->camera.GetView(), );
}


