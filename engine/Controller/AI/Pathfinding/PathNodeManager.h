#pragma once

#include <Debug/DebugDrawables.h>
#include "A_Star.h"	

namespace GaemPathing {

	class PathNodeManager
	{
	public:
		PathNodeManager();
		~PathNodeManager();

		PathNode* AddNode(glm::vec3 position);
		void DeleteNode(PathNode* node);

		void DrawDebug(glm::mat4 projection, glm::mat4 view, Shader* shader);
		
		float GetMaxConnectionDist();

		void SetMaxConnectionDist(float distance);

	private:

		float _maxConnectionDist = 1.0f;
		std::vector<GaemPathing::PathNode*> _nodes;

	};

}