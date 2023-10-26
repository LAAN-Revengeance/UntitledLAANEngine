#pragma once
#include "PathNode.h"
#include <float.h>
#include <vector>

namespace GaemPathing {
	
	struct AS_Node {

		//reference to this star nodes path node
		PathNode* node;

		//previous node
		AS_Node* parent;

		//distance to goal line of sight, ignoring nodes
		float lineOfSight = FLT_MAX;

		//estimated cost to reach this node from start
		float local = FLT_MAX;

		//estimated cost to reach goal from start, passing though this node
		float global = FLT_MAX;

		bool visited = false;
	};

	std::vector<glm::vec3> FindPathA_Star(PathNode* startNode, PathNode* endNode);
}