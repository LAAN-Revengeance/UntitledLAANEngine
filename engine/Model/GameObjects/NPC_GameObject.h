#pragma once
#include <GameObject.h>
#include <AI/Pathfinding/PathNodeManager.h>

using namespace GaemPathing;

class NPC_GameObject : public GameObject
{
public:
	NPC_GameObject();
	~NPC_GameObject();

	void Update(double dt);

	void SetPath(std::vector<glm::vec3>& path);
	void MoveToPoint(PathNode* node);

private:
	

	float _moveSpeed;
	std::vector<glm::vec3> _currentPath;
};

