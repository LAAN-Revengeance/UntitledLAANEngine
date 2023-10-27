#pragma once
#include "PathNode.h"
#include <float.h>
#include <vector>
#include <list>


namespace GaemPathing {

	/**
	*	@Class AS_Node
	*	@brief Represents a node in an A* algorithm
	*
	*	@author Andres Comeros-Ochtman
	*	@version 1.0
	*	@date 26/10/2023
	*/
	struct AS_Node {


		/**
		 * @brief constructor
		 * @param n the node this is an A* representation of 
		 * @param endNode the target location used in the A* algorithm
		*/
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

	/**
	 * @brief returns an array of locations generated using the A* algorithm.
	 * @param start the staring node
	 * @param end the target node
	 * @param nodes all the possible nodes in the network
	 * @return path from start node to the end node(will only contain the location of start node if no path found)
	*/
	std::vector<glm::vec3> FindPathA_Star(PathNode* start, PathNode* end, std::vector<PathNode*>& nodes);
}