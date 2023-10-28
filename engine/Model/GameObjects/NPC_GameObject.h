#pragma once
#include <GameObject.h>
#include <AI/Pathfinding/PathNodeManager.h>

using namespace GaemPathing;

class NPC_GameObject : public GameObject
{
public:
	NPC_GameObject(PathNodeManager* pathManager);
	~NPC_GameObject();

	void Update(double dt);

	void MoveToPoint(PathNode* targetNode,const std::vector<PathNode*> nodes);

private:
	
	void UpdatePathing(double dt);

	PathNodeManager* _pathManager = nullptr;
	float _moveSpeed = 1.0f;
	bool _isMoving = false;
	std::vector<PathNode*> _currentPath;
	PathNode* _currentNode = nullptr;
	PathNode* _targetNode = nullptr;
};

