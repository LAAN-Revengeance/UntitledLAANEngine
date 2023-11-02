#pragma once
#include "State.h"
#include <ResourceManager.h>

class State_Chase : public State
{
public:
	State_Chase();
	State_Chase(std::string&);
	~State_Chase();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent, const Message* message);

private:
	GameObject* target;
	std::string targetName;

	float speed = 20.0f;
	float offset = 2.0f;
};