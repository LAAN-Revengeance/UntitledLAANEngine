#include "NPC_GameObject.h"
#include <Utils/GaemUtils.h>

using namespace GaemPathing;

NPC_GameObject::NPC_GameObject()
{

}

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
	if (!_pathManager)return;
	_targetNode = targetNode;
	
	//find current node
	bool inNetwork = false;
	for (auto& node : nodes)
	{
		if (node->ContainsPoint(position)) {
			_currentNode = node;
			inNetwork = true;
			std::cout << "in network!\n";
			break;
		}
	}

	if (!inNetwork) {
		_isMoving = false;
		std::cout << "not in network, cannot path!\n";
		return;
	}

	//find path required to get there
	_currentPath = GaemPathing::FindPathA_StarPositionsNodes(_currentNode,_targetNode,_pathManager->GetNodes());

	_isMoving = true;
}

void NPC_GameObject::CancelPath()
{
	_isMoving = false;
	_targetNode = nullptr;
	while (!_currentPath.empty()) {
		_currentPath.pop();
	}
}

void NPC_GameObject::SetPathManager(PathNodeManager* pathManager)
{
	_pathManager = pathManager;
}

GaemPathing::PathNode* NPC_GameObject::GetTargetNode()
{
	return _targetNode;
}

GaemPathing::PathNode* NPC_GameObject::GetCurrentNode()
{
	return _currentNode;
}

GaemPathing::PathNode* NPC_GameObject::GetNextNode()
{
	if (_currentPath.empty())
		return nullptr;

	return _currentPath.top();
}

float NPC_GameObject::GetMoveSpeed()
{
	return _moveSpeed;
}

void NPC_GameObject::SetMoveSpeed(float speed)
{
	_moveSpeed = speed;
}

bool NPC_GameObject::GetIsMoving()
{
	return _isMoving;
}

void NPC_GameObject::SetIsMoving(bool isMoving)
{
	_isMoving = isMoving;
}

void NPC_GameObject::UpdatePathing(double dt)
{
	if (!_pathManager)return;


	if (_currentPath.top()->ContainsPoint(position)) {
		_currentNode = _currentPath.top();
		_currentPath.pop();

		if (_currentPath.empty()) {
			std::cout << "reached destination!\n";
			CancelPath();
			return;
		}
	}

	//move towards point
	glm::vec3 offset = (glm::normalize(_currentPath.top()->GetPosition() - GetPosition()) * (float)dt) * _moveSpeed ;
	SetPosition(GetPosition() + offset);

	
	//look at point direction
	LookAt(_currentPath.top()->GetPosition());

}
