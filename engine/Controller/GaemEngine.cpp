#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>
#include <cmath> 
#include <Utils/GaemUtils.h>
#include <AI/Pathfinding/A_Star.h>
#include <time.h>

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

	srand(time(NULL));
	//temp test code
	for (size_t i = 0; i < 16; i++)
	{	
		for (size_t j = 0; j < 16; j++)
		{
			GaemPathing::PathNode* node = new GaemPathing::PathNode({ i * 2,0,j * 2 });
			nodes.push_back(node);

			
			if (rand() % 10 == 0)
				node->SetObstacle(true);
		}
	}

	for (auto& currentNode : nodes)
	{
		for (auto& potentialN : nodes) {
			if (currentNode->GetNeighbours().find(potentialN) == currentNode->GetNeighbours().end() && currentNode != potentialN) {

				float dist = glm::length(currentNode->GetPosition() - potentialN->GetPosition());
				if (dist < 3.0f) {
					currentNode->AddNeighbour(potentialN);
				}
			}
		}
		
	}
	std::vector<glm::vec3> positions = GaemPathing::FindPathA_Star(nodes[0], nodes[nodes.size()-1], nodes);

	path.SetLine(positions);
	path.SetColor({ 1,0,0,1 });
	path.SetWidth(10);
	path.SetPosition({0,2,0});

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


	//temp
	for (auto& node : nodes)
	{
		node->Draw(scene->camera.GetProjection(), scene->camera.GetView(), ResourceManager::Get().GetShader("line"));
		path.Render(scene->camera.GetProjection(), scene->camera.GetView(), ResourceManager::Get().GetShader("line"));
	}
}


