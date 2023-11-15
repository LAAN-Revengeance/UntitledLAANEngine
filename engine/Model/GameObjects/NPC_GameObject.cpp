#include "NPC_GameObject.h"
#include <Utils/GaemUtils.h>
#include <Utils/DebugLogger.h>

using namespace GaemPathing;

NPC::NPC()
{

}

NPC::NPC(PathNodeManager* pathManager) :
	_pathManager(pathManager)
{

}

NPC::~NPC()
{

}

void NPC::Update(double dt)
{
	if (_isMoving)
		UpdatePathing(dt);

	for (auto& emotion : emotions)
	{
		if(emotion.emotionStrength > 0)
			emotion.emotionStrength -= dt * 0.01;
		if (emotion.emotionStrength < 0)
			emotion.emotionStrength = 0;
	}
}

void NPC::AddEmotion(std::string name)
{
	this->emotions.push_back({ name });
}

std::vector<Emotion>& NPC::GetEmotions()
{
	return this->emotions;
}

Emotion& NPC::GetEmotion(std::string emotionName)
{
	for (int i = 0; i < this->emotions.size(); i++)
	{
		if (this->emotions[i].emotion == emotionName)
			return this->emotions[i];
	}
	//DebugLogger::Log(GAEM_LOG, "could not find emotion " + emotionName, name);
	return _emptyEmotion;
}

Personality& NPC::GetPersonality()
{
	return this->personality;
}

void NPC::SetPersonality(Personality personality)
{
	this->personality = personality;
}

void NPC::SetEmotionStrength(std::string emotionName, float value)
{
	for (int i = 0; i < emotions.size(); i++)
	{
		if (emotions[i].emotion == emotionName)
			emotions[i].emotionStrength = value;
	}
}

void NPC::SetReactionStrength(std::string emotionName, float value)
{
	for (int i = 0; i < emotions.size(); i++)
	{
		if (emotions[i].emotion == emotionName)
			emotions[i].reactionStrength = value;
	}
}

void NPC::MoveToPoint(PathNode* targetNode)
{
	if (!_pathManager)return;
	_targetNode = targetNode;
	
	//find current node
	bool inNetwork = false;
	for (auto& node : _pathManager->GetNodes())
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
			//DebugLogger::Log(GAEM_LOG, "Pathing to last visited node", name);
			//_returningToNetwork = true;
			_isMoving = true;
		}
		else {
			_currentNode = FindClosestNode();
			if (_currentNode == nullptr) {
				DebugLogger::Log(GAEM_LOG, "Pathing to closest node", name);
				_isMoving = false;
				return;
			}
			_isMoving = true;
			_returningToNetwork = true;
		}
		_isMoving = true;
	}

	//find path required to get there
	_currentPath = GaemPathing::FindPathA_Star(_currentNode,_targetNode,_pathManager->GetNodes());

	_isMoving = true;
}

GaemPathing::PathNode* NPC::FindClosestNode()
{
	if (_pathManager->GetNodes().empty()) {
		DebugLogger::Log(GAEM_ERROR, "Could not find closest  node", name);
		return nullptr;
	}
		
	PathNode* closestNode = _pathManager->GetNodes()[0];
	float squareDist = glm::distance2(closestNode->GetPosition(), position);

	for (auto& node : _pathManager->GetNodes())
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

GaemPathing::PathNode* NPC::FindFurthestNode()
{
	if (_pathManager->GetNodes().empty()) {
		DebugLogger::Log(GAEM_ERROR, "Could not find furthest node", name);
		return nullptr;
	}
	

	PathNode* furthestNode = _pathManager->GetNodes()[0];
	float squareDist = glm::distance2(furthestNode->GetPosition(), position);

	for (auto& node : _pathManager->GetNodes())
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

GaemPathing::PathNode* NPC::FindRandomNode()
{
	if (_pathManager->GetNodes().empty()) {
		DebugLogger::Log(GAEM_ERROR, "No nodes to select from", name);
		return nullptr;
	}

	srand(static_cast<unsigned int>(time(nullptr)) * ID);
	int randNum = rand() % _pathManager->GetNodes().size();

	PathNode* randomNode = _pathManager->GetNodes()[randNum];

	DebugLogger::Log(GAEM_LOG, "Random node found", name);

	return randomNode;
}

void NPC::CancelPath()
{
	_isMoving = false;
	_targetNode = nullptr;
	while (!_currentPath.empty()) {
		_currentPath.pop();
	}
}

void NPC::SetPathManager(PathNodeManager* pathManager)
{
	_pathManager = pathManager;
}

GaemPathing::PathNode* NPC::GetTargetNode()
{
	return _targetNode;
}

GaemPathing::PathNode* NPC::GetCurrentNode()
{
	return _currentNode;
}

GaemPathing::PathNode* NPC::GetNextNode()
{
	if (_currentPath.empty())
		return nullptr;

	return _currentPath.top();
}

float NPC::GetMoveSpeed()
{
	return _moveSpeed;
}

void NPC::SetMoveSpeed(float speed)
{
	_moveSpeed = speed;
}

bool NPC::GetIsMoving()
{
	return _isMoving;
}

void NPC::SetIsMoving(bool isMoving)
{
	_isMoving = isMoving;
}

GameObject* NPC::GetLastInterracted()
{
	return _lastInteracted;
}

void NPC::SetLastInterracted(GameObject* go)
{
	_lastInteracted = go;
}

void NPC::UpdatePathing(double dt)
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
		MoveToPoint(_targetNode);
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
	//LookAt(_currentPath.top()->GetPosition());


	//lookat the right og NPC, for demo only(md2 model is rotated wrong so bandaid fix here)
	glm::vec3 toTarget = (_currentPath.top()->GetPosition() - position);
	LookAt(glm::cross({ 0,20,0 }, toTarget));

}
