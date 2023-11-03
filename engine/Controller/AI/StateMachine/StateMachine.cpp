#include "StateMachine.h"
StateMachine::StateMachine(GameObject* nOwner) :
	owner(nOwner),
	currentState(nullptr),
	globalState(nullptr),
	previousState(nullptr)
{}

void StateMachine::ChangeState(State& nState)
{
	if(currentState && currentState != &nState)
		currentState->Exit(*owner);

	previousState = currentState;
	currentState = &nState;
	currentState->Enter(*owner);
}

void StateMachine::ChangeGlobalState(State& nState)
{
	if(globalState && globalState != &nState)
		globalState->Exit(*owner);

	globalState = &nState;
	globalState->Enter(*owner);
}

void StateMachine::ChangePreviousState(State& nState)
{
	previousState = &nState;
	previousState->Enter(*owner);
}

void StateMachine::RevertState()
{
	if (previousState)
		ChangeState(*previousState);
}

void StateMachine::Update(double dt)
{
	if(currentState)
		currentState->Update(*owner, dt);
	if (globalState)
		globalState->Update(*owner, dt);
}

void StateMachine::ReceiveMessage(const Message* message)
{
	if(currentState)
		currentState->ProcessMessage(owner, message);
	if(globalState)
		globalState->ProcessMessage(owner, message);
}

State* StateMachine::GetState()
{
	return currentState;
}

State* StateMachine::GetGlobalState()
{
	return globalState;
}

State* StateMachine::GetPreviousState()
{
	return previousState;
}
