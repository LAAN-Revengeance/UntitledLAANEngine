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
		void AddNeighbour(PathNode* neighbour,float cost);
		void RemoveNeighbour(PathNode* neighbour);
		void ClearNeighbours();

		const std::map<PathNode*, float>& GetNeighbours();

		void Draw(glm::mat4 projection, glm::mat4 view, Shader* shader);

		void UpdateConnections(std::vector<PathNode*> nodes, float distance);
		void UpdateConnections();
		void UpdateModel();

		unsigned int GetID();
		void SetID(unsigned int id);
	private:

		glm::vec3 _position = { 0,0,0 };

		bool _obstacle = false;

		//stores pointer to adjacent node and the cost of conection
		std::map<PathNode*, float> _neighbours;

		//debug rendering
		std::map<PathNode*, GaemGizmo::Line*> lines;
		GaemGizmo::Box box;

		//node ID, used as refernce for serialization
		unsigned int ID = 0;

		friend class PathNodeManager;
	};
}