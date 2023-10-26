#include "GaemEngine.h"
#include <string.h>
#include <iostream>
#include <functional>
#include <cmath> 
#include <Utils/GaemUtils.h>

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

	for (size_t i = 0; i < 10; i++)
	{
		
		for (size_t j = 0; j < 10; j++)
		{
			GaemPathing::PathNode* node = new GaemPathing::PathNode({ i,0,j });
			nodes.push_back(node);

			GaemGizmo::Box* nodeIndicator = new GaemGizmo::Box({ i,0,j }, {0.3f,0.3f,0.3f});
			nodeIndicator->SetColor({0,0,1,1});
			gizmos.push_back(nodeIndicator);

			for (auto& n : nodes)
			{
				if (n->_neighbours.find(node) == n->_neighbours.end() && n != node)
				{
					float dist = glm::length(n->_position - node->_position);
					if (dist < 1.1f) {
						node->_neighbours.insert({ n, dist });
						//std::cout << "cum\n";
						gaemutils::PrintVec3("pos1",n->_position);
						gaemutils::PrintVec3("pos2", node->_position);
						std::cout << "--------\n";
						std::vector<glm::vec3>pos({ n->_position, node->_position });
						GaemGizmo::Line* connectionIndicator = new GaemGizmo::Line(pos, {0,0,0});
						connectionIndicator->SetColor({ 1,0,0,1.0 });
						gizmos.push_back(connectionIndicator);
					}
				}

			}
		}

	}

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

	for (auto& gizmo : gizmos)
	{
		gizmo->Render(scene->camera.GetProjection(), scene->camera.GetView(),ResourceManager::Get().GetShader("line"));
	}
}


