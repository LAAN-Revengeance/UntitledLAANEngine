#pragma once
#include <GameObject.h>
#include <AI/Pathfinding/PathNodeManager.h>
#include <AI/Emotion/Emotion.h>
#include <Ai/Emotion/Personality.h>

class NPC_GameObject : public GameObject
{
public:
	NPC_GameObject();
	NPC_GameObject(GaemPathing::PathNodeManager* pathManager);
	~NPC_GameObject();

	void Update(double dt);

	//#########################################
	//#		EMOTION MODELING/FCM
	//#########################################

	void AddEmotion(std::string name);
	void AddEmotion(std::string name, float strength);
	std::vector<Emotion>& GetEmotions();
	Emotion& GetEmotion(std::string emotionName);
	Personality& GetPersonality();
	void SetPersonality(Personality personality);
	void SetEmotionStrength(std::string emotionName, float value);
	void SetReactionStrength(std::string emotionName, float value);

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
	
	//Emotion
	std::vector<Emotion> emotions;
	Personality personality;

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

