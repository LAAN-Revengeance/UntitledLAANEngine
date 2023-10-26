#pragma once
#include <glm/glm.hpp>
#include <map>

namespace GaemPathing{

	struct PathNode
	{

		PathNode();
		PathNode(glm::vec3 positon);


		bool obstacle = false;
		glm::vec3 _position = { 0,0,0 };

		//stores pointer to adjacent node and the cost of conection
		std::map<PathNode*, float> _neighbours;

	};
}