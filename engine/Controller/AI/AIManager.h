#pragma once
#include <AI/Message/MessageDispatcher.h>
/**
*	@Class AIManager
*	@brief manages state machines and executes their behaviours
*	every AI update cycle.
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 25/05/2023
*/
class AIManager
{
public:
		/**
		*	@brief get the singleton instance of AI manager
		*	@return AIManaer reference to the singleton instance
		*/
	static AIManager& Get();

		/**
		*	@brief Updates all AI agents
		*	@param deltaTime time since last frame
		*	@return void
		*/
	void UpdateAgents(double deltaTime);

		/**
		*	@brief Set the scene AI will be coordinated in
		*	@param nScene scene to be used
		*	@return void
		*/
	void Init(Scene* nScene);

		/**
		*	@brief Store a state in internal map for later access
		*	@param stateName name of state
		*	@param nState pointer to state being added
		*	@return void
		*/
	void AddState(const std::string& stateName, State*  nState);

		/**
		*	@brief Get a state from storage
		*	@param stateName name of state to be retrieved
		*	@return State* pointer to state with name StateName
		*/
	State* GetState(const std::string& stateName);

		/**
		*	@brief GLFW callback for window resizing. recalulates camera properties and viewport
		*	@param delay how long to wait before sending the message
		*	@param sender sender ID
		*	@param reciver reciever ID
		*	@param type type ID
		*	@return void
		*/
	void SendMessage(double delay, int sender, int receiver, int type);

		/**
		*	@brief Get the name of a state
		*	@param state the state having its name looked up
		*	@return string name of state
		*/
	std::string GetStateKey(State* state);
private:

		///Pointer to message dispatcher
	Dispatcher* msgDispatcher;
		///Pointer to main scene
	Scene* scene;
		///Storage of all states added
	std::map<std::string, State*> states;

		///How much time has passed since last AI update
	double accumilator = 0;
		///How many miliseconds between AI Updates
	double updateTime = 0;

	//singleton
	AIManager();
	~AIManager();
	AIManager(const AIManager&) = delete;
	AIManager& operator = (const AIManager&) = delete;
};
