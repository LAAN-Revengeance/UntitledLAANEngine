#pragma once
#include "State.h"
#include <ResourceManager.h>

class State_Wander : public State 
{
public:
	State_Wander();
	~State_Wander();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent, const Message* message);

private:
	glm::vec3 GetWanderTarget();
};