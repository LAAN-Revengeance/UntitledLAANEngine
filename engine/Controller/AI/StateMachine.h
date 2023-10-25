#pragma once
#include "States/State.h"

class GameObject;
/**
*	@Class StateMachine
*	@brief AI state Machine that can change the properties of its
*	owner GameObject based on its current state
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 25/05/2023
*/
class StateMachine
{
public:


		/**
		*	@brief Constructor, state machine requires an owner
		*	@param nOwner owner of this state machine
		*/
	StateMachine(GameObject* nOwner);

		/**
		*	@brief destructor
		*/
	~StateMachine() {}

		/**
		*	@brief Change this state machines current state
		*	@param nState refernce to the new state
		*	@return void
		*/
	void ChangeState(State& nState);

		/**
		*	@brief Change this state machines global state
		*	@param nState refernce to the new state
		*	@return void
		*/
	void ChangeGlobalState(State& nState);

		/**
		*	@brief Change this state machines previous state
		*	@param nState refernce to the new state
		*	@warning not recomended to use, using will remove knownledge of previous state
		*	@return void
		*/
	void ChangePreviousState(State& nState);

		/**
		*	@brief Change state to previous state
		*	@return void
		*/
	void RevertState();

		/**
		*	@brief Run both state and global state update methods using
		*	owner as parameter
		*	@param dt time since last update
		*	@return void
		*/
	void Update(double dt);

		/**
		*	@brief Run the state and global staet message methods using
		*	owner and message as parameters
		*	@param message message being recieved
		*	@return void
		*/
	void ReceiveMessage(const Message* message);

		///Get Current State
	State* GetState();
		///Get Global State
	State* GetGlobalState();
		///Get Previous State
	State* GetPreviousState();

private:

	///This State Machines owner
	GameObject* owner;

	///Current state
	State* currentState;

	///Global state - this state is run alongside current state
	State* globalState;

	///Last state currentState was assigned to
	State* previousState;
};