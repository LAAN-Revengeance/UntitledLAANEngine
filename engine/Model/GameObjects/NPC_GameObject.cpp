#include "NPC_GameObject.h"

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
			break;
		}
	}

	if (!inNetwork) {
		_isMoving = false;
		return;
	}

	//find path required to get there
	_currentPath = GaemPathing::FindPathA_StarPositionsNodes(_currentNode,_targetNode,_pathManager->GetNodes());
	_nextNode = _currentNode;

	_isMoving = true;
}

void NPC_GameObject::SetPathManager(PathNodeManager* pathManager)
{
	_pathManager = pathManager;
}

void NPC_GameObject::UpdatePathing(double dt)
{
	if (!_pathManager)return;

	if (_targetNode == nullptr) {
		_isMoving = false;
		while (!_currentPath.empty())
			_currentPath.pop();
	}
	
	//check if at next node yet
	if (_nextNode->ContainsPoint(position)) {

		//reached destination
		if (_nextNode == _targetNode) {
			_isMoving = false;
			_targetNode = nullptr;
			_nextNode = nullptr;
			return;
		}

		_currentNode = _nextNode;

		_nextNode = _currentPath.top();
		_currentPath.pop();
	}

	//move towards point
	glm::vec3 offset = glm::normalize(GetPosition() + _nextNode->GetPosition()) * _moveSpeed;
	SetPosition(GetPosition() + offset);

	//look at point direction
	LookAt(_nextNode->GetPosition());

}
