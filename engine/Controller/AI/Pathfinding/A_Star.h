#pragma once
#include "PathNode.h"
#include <float.h>
#include <vector>
#include <list>

namespace GaemPathing {
	
	struct AS_Node {

		AS_Node(PathNode* n, PathNode* endNode) {
			node = n;
			lineOfSight = glm::length(n->GetPosition() - endNode->GetPosition());
		}

		//reference to this star nodes path node
		PathNode* node = nullptr;

		//previous node
		AS_Node* parent = nullptr;

		//distance to goal line of sight, ignoring nodes
		float lineOfSight = FLT_MAX;

		//estimated cost to reach this node from start
		float local = FLT_MAX;

		//estimated cost to reach goal from start, passing though this node
		float global = FLT_MAX;

		bool visited = false;
	};

	std::vector<glm::vec3> FindPathA_Star(PathNode* start, PathNode* end, std::vector<PathNode*>& nodes);
}