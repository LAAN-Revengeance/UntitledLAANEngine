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

	void MoveToPoint(PathNode* node);

private:
	
	float _moveSpeed = 1.0f;
	std::vector<PathNode*> _currentPath;
	PathNode* _currentNode = nullptr;
	PathNode* _targetNode = nullptr;
};

