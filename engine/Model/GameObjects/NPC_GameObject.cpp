#include "NPC_GameObject.h"
#include <Utils/GaemUtils.h>
#include <Utils/DebugLogger.h>

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

	updateFunction.Execute(*this);
}

void NPC_GameObject::AddEmotion(std::string name)
{
	this->emotions.push_back({ name });
}

void NPC_GameObject::AddEmotion(std::string name, float strength)
{
	this->emotions.push_back({ name, strength });
}

std::vector<Emotion> NPC_GameObject::GetEmotions()
{
	return this->emotions;
}

Emotion& NPC_GameObject::GetEmotion(std::string emotionName)
{
	for (int i = 0; i < this->emotions.size(); i++)
	{
		if (this->emotions[i].emotion == emotionName)
			return this->emotions[i];
	}

	std::cout << "Error: no emotion with the name " << emotionName << " was found." << std::endl;

	Emotion empty;

	return empty;
}

Personality NPC_GameObject::GetPersonality()
{
	return this->personality;
}

void NPC_GameObject::SetPersonality(Personality personality)
{
	this->personality = personality;
}

void NPC_GameObject::SetEmotionStrength(std::string emotionName, float value)
{
	for (int i = 0; i < emotions.size(); i++)
	{
		if (emotions[i].emotion == emotionName)
			emotions[i].emotionStrength;
	}
}

void NPC_GameObject::SetReactionStrength(std::string emotionName, float value)
{
	for (int i = 0; i < emotions.size(); i++)
	{
		if (emotions[i].emotion == emotionName)
			emotions[i].reactionStrength;
	}
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
		DebugLogger::Log(GAEM_LOG, "Not in network, searching for available node", name);
		if (_currentNode) {
			DebugLogger::Log(GAEM_LOG, "Pathing to last visited node", name);
			_returningToNetwork = true;
			_isMoving = true;
		}
		else {
			_currentNode = FindClosestNode(_pathManager->GetNodes());
			if (_currentNode == nullptr) {
				DebugLogger::Log(GAEM_LOG, "Pating to closest node", name);
				_isMoving = false;
				return;
			}
			_isMoving = true;
			_returningToNetwork = true;
		}
		_isMoving = true;
	}

	//find path required to get there
	_currentPath = GaemPathing::FindPathA_StarPositionsNodes(_currentNode,_targetNode,_pathManager->GetNodes());

	_isMoving = true;
}

GaemPathing::PathNode* NPC_GameObject::FindClosestNode(const std::vector<GaemPathing::PathNode*> nodes)
{
	if (nodes.empty()) {
		DebugLogger::Log(GAEM_ERROR, "Could not find closest  node", name);
		return nullptr;
	}
		
	PathNode* closestNode = nodes[0];
	float squareDist = glm::distance2(closestNode->GetPosition(), position);

	for (auto& node : nodes)
	{
		float sqrTestDist = glm::distance2(node->GetPosition(), position);
		if (sqrTestDist < squareDist) {
			
			closestNode = node;
			squareDist = sqrTestDist;
		}
	}
	DebugLogger::Log(GAEM_LOG, "Closest node found", name);

	return closestNode;
}

GaemPathing::PathNode* NPC_GameObject::FindFurthestNode(const std::vector<GaemPathing::PathNode*> nodes)
{
	if (nodes.empty()) {
		DebugLogger::Log(GAEM_ERROR, "Could not find furthest node", name);
		return nullptr;
	}
	

	PathNode* furthestNode = nodes[0];
	float squareDist = glm::distance2(furthestNode->GetPosition(), position);

	for (auto& node : nodes)
	{
		float sqrTestDist = glm::distance2(node->GetPosition(), position);
		if (sqrTestDist > squareDist) {

			furthestNode = node;
			squareDist = sqrTestDist;
		}
	}
	DebugLogger::Log(GAEM_LOG, "Furthest node found", name);

	return furthestNode;
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
	if (_returningToNetwork) {
		//move towards point
		glm::vec3 offset = (glm::normalize(_currentNode->GetPosition() - GetPosition()) * (float)dt) * _moveSpeed;
		SetPosition(GetPosition() + offset);
		if (_currentNode->ContainsPoint(position)) {
			_returningToNetwork = false;
			DebugLogger::Log(GAEM_LOG, "Rejoined network", name);
		}
		return;
	}

	if (!_pathManager)return;

	if (_currentPath.top()->GetObstacle()) {
		MoveToPoint(_targetNode, _pathManager->GetNodes());
		return;
	}

	if (_currentPath.top()->ContainsPoint(position)) {
		_currentNode = _currentPath.top();
		_currentPath.pop();

		if (_currentPath.empty()) {
			DebugLogger::Log(GAEM_LOG, "Destination Reached", name);
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
