#pragma once
#include "PathNode.h"
#include <limits>
#include <vector>

namespace GaemPathing {
	
	struct AS_Node {

		//reference to this star nodes path node
		PathNode* node;

		//previous node
		AS_Node* parent;

		//distance to goal line of sight, ignoring nodes
		float lineOfSight = std::numeric_limits<float>::max();

		//estimated cost to reach this node from start
		float local = std::numeric_limits<float>::max();

		//estimated cost to reach goal from start, passing though this node
		float global = std::numeric_limits<float>::max();

		bool visited = false;
	};

	std::vector<glm::vec3> FindPathA_Star(PathNode* startNode, PathNode* endNode);
}