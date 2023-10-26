#pragma once
#include <glm/glm.hpp>
#include <map>

namespace GaemPath{

	class PathNode
	{
	public:
		PathNode();
		PathNode(glm::vec3 positon);

		void AddNeighbour(PathNode* neighbour);
		void RemoveNeighbour(PathNode* neighbour);
		void ClearNeighbours();

	private:
		glm::vec3 _position = {0,0,0};

		//stores pointer to adjacent node and the cost of conection
		std::map<PathNode*, float> _neighbours;
	};
}