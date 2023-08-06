#pragma once
#include "State.h"
#include <ResourceManager.h>
#include <AI/AIManager.h>

class State_Evade : public State
{
public:
	State_Evade();
	State_Evade(std::string& nTargetName);
	~State_Evade();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent, const Message* message);

private:
	GameObject* target;
	std::string targetName;
	State* stateWander;
};