#include "NPC_GameObject.h"

NPC_GameObject::NPC_GameObject(PathNodeManager* pathManager) :
	_pathManager(pathManager)
{

}

NPC_GameObject::~NPC_GameObject()
{

}

void NPC_GameObject::Update(double dt)
{
	if (_isMoving)
		UpdatePathing(dt);
}

void NPC_GameObject::MoveToPoint(PathNode* targetNode, const std::vector<PathNode*> nodes)
{
	_targetNode = targetNode;
	
	//find current node

	//find path required to get there
	_currentPath = GaemPathing::FindPathA_StarPositionsNodes(_currentNode,_targetNode,_pathManager->GetNodes());

	_isMoving = true;
}

void NPC_GameObject::UpdatePathing(double dt)
{
	if (_targetNode == nullptr) {
		_isMoving = false;
		_currentPath.clear();
	}
		

	LookAt(_targetNode->GetPosition());

}
