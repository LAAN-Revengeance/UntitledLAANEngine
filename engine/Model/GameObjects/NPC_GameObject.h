#pragma once
#include <GameObject.h>
#include <AI/Pathfinding/PathNodeManager.h>

class NPC_GameObject : public GameObject
{
public:
	NPC_GameObject();
	NPC_GameObject(GaemPathing::PathNodeManager* pathManager);
	~NPC_GameObject();

	void Update(double dt);

	void MoveToPoint(GaemPathing::PathNode* targetNode,const std::vector<GaemPathing::PathNode*> nodes);
	void CancelPath();

	void SetPathManager(GaemPathing::PathNodeManager* pathManager);



	GaemPathing::PathNode* GetTargetNode();
	GaemPathing::PathNode* GetCurrentNode();
	GaemPathing::PathNode* GetNextNode();


	float GetMoveSpeed();
	void SetMoveSpeed(float speed);
	bool GetIsMoving();
	void SetIsMoving(bool isMoving);

private:
	
	void UpdatePathing(double dt);

	GaemPathing::PathNodeManager* _pathManager = nullptr;
	float _moveSpeed = 1.0f;
	bool _isMoving = false;
	std::stack<GaemPathing::PathNode*> _currentPath;

	GaemPathing::PathNode* _currentNode = nullptr;
	GaemPathing::PathNode* _targetNode = nullptr;
};

