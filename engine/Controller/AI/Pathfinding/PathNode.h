#pragma once
#include <glm/glm.hpp>
#include <Debug/DebugDrawables.h>
#include <map>

namespace GaemPathing{

	class PathNode
	{
	public:
		PathNode();
		PathNode(glm::vec3 positon);

		void SetPosition(glm::vec3 position);
		void SetObstacle(bool obstacle);

		glm::vec3 GetPosition();
		bool GetObstacle();


		void AddNeighbour(PathNode* neighbour);
		void RemoveNeighbour(PathNode* neighbour);
		void ClearNeighbours();

		const std::map<PathNode*, float>& GetNeighbours();

		void Draw(glm::mat4 projection, glm::mat4 view, Shader* shader);

	private:
		void UpdateModel();

		glm::vec3 _position = { 0,0,0 };

		bool _obstacle = false;
		GaemGizmo::Box box;
		//stores pointer to adjacent node and the cost of conection
		std::map<PathNode*, float> _neighbours;
		std::map<PathNode*, GaemGizmo::Line*> lines;
	};
}