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

	//#########################################
	//#		AFFORDANCES
	//#########################################

	//affordance functions go here

	//#########################################
	//#		EMOTION MODELING/FCM
	//#########################################

	//emotions go here

	//#########################################
	//#		NAVIGATION/PATHFINDING
	//#########################################
	void MoveToPoint(GaemPathing::PathNode* targetNode,const std::vector<GaemPathing::PathNode*> nodes);
	void CancelPath();
	GaemPathing::PathNode* FindClosestNode(const std::vector<GaemPathing::PathNode*> nodes);
	GaemPathing::PathNode* FindFurthestNode(const std::vector<GaemPathing::PathNode*> nodes);
	
	GaemPathing::PathNode* GetTargetNode();
	GaemPathing::PathNode* GetCurrentNode();
	GaemPathing::PathNode* GetNextNode();

	void SetPathManager(GaemPathing::PathNodeManager* pathManager);
	float GetMoveSpeed();
	void SetMoveSpeed(float speed);
	bool GetIsMoving();
	void SetIsMoving(bool isMoving);

private:
	
	//Affordance

	//Emotion

	//Navigation
	void UpdatePathing(double dt);
	GaemPathing::PathNodeManager* _pathManager = nullptr;
	float _moveSpeed = 1.0f;
	bool _isMoving = false;
	bool _returningToNetwork = false;
	std::stack<GaemPathing::PathNode*> _currentPath;

	GaemPathing::PathNode* _currentNode = nullptr;
	GaemPathing::PathNode* _targetNode = nullptr;
};
