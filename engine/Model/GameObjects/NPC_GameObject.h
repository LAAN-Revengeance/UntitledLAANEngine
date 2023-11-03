#pragma once
#include <GameObject.h>
#include <AI/Pathfinding/PathNodeManager.h>
#include <AI/Emotion/Emotion.h>
#include <Ai/Emotion/Personality.h>

/**
*	@Class NPC
*	@brief Extends GameObject to provide Intelligent behaviour using pathfinding
*	and emotion modelling using the OCC model of emotions.
*
*	@author Andres Comeros-Ochtman, Aidan O'Connor
*	@version 1.0
*	@date 15/08/2023
*/
class NPC : public GameObject
{
public:

	/**
	 * @brief constructor
	*/
	NPC();

	/**
	 * @brief constructor with initalization for pathfinding system
	 * @param pathManager pathmanger to use for pathfinding
	*/
	NPC(GaemPathing::PathNodeManager* pathManager);

	/**
	 * @brief destructor
	*/
	~NPC();

	/**
	 * @brief Updates pathfinding, emotion and state machine behaviours
	 * @param dt 
	*/
	void Update(double dt);

	/**
	 * @brief Add an emotional state to this agent
	 * @param name emotion name
	*/
	void AddEmotion(std::string name);

	/**
	 * @brief Get the emotion array from this agent
	 * @return the emotion array
	*/
	std::vector<Emotion>& GetEmotions();

	/**
	 * @brief get a specific emotion from this agent
	 * @param emotionName name of the emotion being retrieved
	 * @return emotion woth emotionName as its name
	*/
	Emotion& GetEmotion(std::string emotionName);

	/**
	 * @brief return the personality information of this agent
	 * @return personality info
	*/
	Personality& GetPersonality();

	/**
	 * @brief Set this agents personality data
	 * @param personality this agents personality
	*/
	void SetPersonality(Personality personality);

	/**
	 * @brief Set the strenght of an emotion this agent is feeling
	 * @param emotionName name of the emoiton to modify
	 * @param value new emotion value
	*/
	void SetEmotionStrength(std::string emotionName, float value);

	/**
	 * @brief Set the reacton strength of a specified emotion
	 * @param emotionName the reaction value to change
	 * @param value new reaction strength
	*/
	void SetReactionStrength(std::string emotionName, float value);

	/**
	 * @brief Set a node to pathfind to
	 * @param targetNode the node to attempt to move to
	*/
	void MoveToPoint(GaemPathing::PathNode* targetNode);

	/**
	 * @brief cancels this agent current movement to a node
	*/
	void CancelPath();

	/**
	 * @brief find the closest node to this agent
	 * @return the closest node
	*/
	GaemPathing::PathNode* FindClosestNode();

	/**
	 * @brief find the furthest node to this agent
	 * @return the furthest node
	*/
	GaemPathing::PathNode* FindFurthestNode();

	/**
	 * @brief select a random node from the current node manager
	 * @return a random node
	*/
	GaemPathing::PathNode* FindRandomNode();
	
	/**
	 * @brief get the node this agent is attempting to get to
	 * @return the final node in this agents path
	*/
	GaemPathing::PathNode* GetTargetNode();

	/**
	 * @brief get the last node entered
	 * @return last node this agent entered
	*/
	GaemPathing::PathNode* GetCurrentNode();

	/**
	 * @brief get the node this agent is currently moving towards
	 * @return next node in this agents path
	*/
	GaemPathing::PathNode* GetNextNode();

	/**
	 * @brief Set the node manager used for pathfinding
	 * @param pathManager the path manager this agent will use to pathfind
	*/
	void SetPathManager(GaemPathing::PathNodeManager* pathManager);

	/**
	 * @brief get this agents move speed
	 * @return the speed of this agent
	*/
	float GetMoveSpeed();

	/**
	 * @brief Set the move speed this agent will move between nodes
	 * @param speed move speed in m/s
	*/
	void SetMoveSpeed(float speed);

	/**
	 * @brief Get if this agent is currently moving using the pathfinding system
	 * @return if this agent is pathfinding
	*/
	bool GetIsMoving();

	/**
	 * @brief Set if this agent should continue to pathfind
	 * @param isMoving if this agent should move
	*/
	void SetIsMoving(bool isMoving);

	/**
	 * @brief Return the last interracted object
	 * @return object last interracted with
	*/
	GameObject* GetLastInterracted();

	/**
	 * @brief Set the last interracted object
	 * @param go object last interracted with
	*/
	void SetLastInterracted(GameObject* go);
private:

	///record of this agents emoitonal state
	std::vector<Emotion> emotions;

	//traits that affect emotional responses to actions
	Personality personality;

	///blank emotion to return if an emoiton does not exist
	Emotion _emptyEmotion;

	///update this agents movement if it currently has a target node
	void UpdatePathing(double dt);

	///current node network used to pathfind
	GaemPathing::PathNodeManager* _pathManager = nullptr;

	///how fast this npc can move in m/s
	float _moveSpeed = 1.0f;

	//if currently pathfinding to a node
	bool _isMoving = false;

	//if this npc is currently returing to the node networkk
	bool _returningToNetwork = false;

	///each node required to visit to reach the target node
	std::stack<GaemPathing::PathNode*> _currentPath;

	///last node this agent has visited
	GaemPathing::PathNode* _currentNode = nullptr;

	///the node this agent is attempting to reach
	GaemPathing::PathNode* _targetNode = nullptr;

	///records last object interacted with
	GameObject* _lastInteracted = nullptr;
};

