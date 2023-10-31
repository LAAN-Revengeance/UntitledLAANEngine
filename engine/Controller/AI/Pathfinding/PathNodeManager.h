#pragma once

#include <Debug/DebugDrawables.h>
#include "A_Star.h"	

namespace GaemPathing {

	/**
	*	@Class PathNodeManager
	*	@brief Manages a network of nodes that can be used for pathfinding
	*
	*	@author Andres Comeros-Ochtman
	*	@version 1.0
	*	@date 26/10/2023
	*/
	class PathNodeManager
	{
	public:

		///constructor
		PathNodeManager();

		///destructor
		~PathNodeManager();

		/**
		 * @brief adds a node to the network
		 * @param position location of the node
		 * @param obstacle if the node can be traversed
		 * @return the node added
		*/
		PathNode* AddNode(glm::vec3 position, bool obstacle);

		/**
		 * @brief remove a node from the network
		 * @param node pointer to the node to remove
		*/
		void DeleteNode(PathNode* node);

		/**
		 * @brief renders a representation of the network
		 * @param projection projection matrix
		 * @param view view matrix
		 * @param shader shader used to render the representation
		*/
		void DrawDebug(glm::mat4 projection, glm::mat4 view, Shader* shader);
		
		/**
		 * @brief Get the maximum distance for a connection for new nodes added
		 * @return the max distance
		*/
		float GetMaxConnectionDist();

		/**
		 * @brief Set the maximum distance for a connection for new nodes added
		 * @param distance the max distance
		*/
		void SetMaxConnectionDist(float distance);

		/**
		 * @brief updates the connections between nodes based on _maxConnectionDistance
		*/
		void UpdateNodes();

		/**
		 * @brief return reference to the netowork nodes
		 * @return vector of nodes from the network
		*/
		const std::vector<GaemPathing::PathNode*>& GetNodes();

	private:

		///represents the maximum distance two nodes can be for a connection to be formed
		float _maxConnectionDist = 5.0f;

		///all nodes in the network
		std::vector<GaemPathing::PathNode*> _nodes;

		///unique ID to assign to next node added to the network
		unsigned int _nextID = 1;

		///map of IDs and nodes. used in serialization
		std::map<unsigned int, PathNode*> _idMap;

		friend class SceneLoader;
	};

}