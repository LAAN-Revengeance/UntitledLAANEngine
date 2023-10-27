#pragma once

#include <Debug/DebugDrawables.h>
#include "A_Star.h"	

namespace GaemPathing {

	class PathNodeManager
	{
	public:
		PathNodeManager();
		~PathNodeManager();

		PathNode* AddNode(glm::vec3 position, bool obstacle);
		void DeleteNode(PathNode* node);

		void DrawDebug(glm::mat4 projection, glm::mat4 view, Shader* shader);
		
		float GetMaxConnectionDist();

		void SetMaxConnectionDist(float distance);

		void UpdateNodes();

		std::vector<GaemPathing::PathNode*>& GetNodes();

	private:

		float _maxConnectionDist = 5.0f;
		std::vector<GaemPathing::PathNode*> _nodes;

		unsigned int nextID = 1;
		std::map<unsigned int, PathNode*> _idMap;

		friend class SceneLoader;
	};

}