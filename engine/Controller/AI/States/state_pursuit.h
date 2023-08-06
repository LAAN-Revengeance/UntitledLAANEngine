#pragma once

#include "State.h"
#include <ResourceManager.h>

class State_Pursuit : public State
{
public:
	State_Pursuit();
	State_Pursuit(std::string& nTargetName);
	~State_Pursuit();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent, const Message* message);

private:
	GameObject* target;
	std::string targetName;
};