#pragma once
#include "State.h"
#include <ResourceManager.h>
#include <AI/AIManager.h>

class State_Patrol : public State
{
public:
	State_Patrol();
	State_Patrol(std::vector<glm::vec3>& nWaypoints);
	~State_Patrol();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent, const Message* message);

	//generate random waypoints
	void GenRandWaypoints(float min, float max, int count);

private:
	//generic waypoints if none assigned
	std::vector<glm::vec3> waypoints;

};