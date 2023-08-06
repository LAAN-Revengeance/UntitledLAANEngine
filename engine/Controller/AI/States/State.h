#pragma once
#include<AI/Message/Message.h>

class GameObject;
class State
{
public:
	virtual ~State() {};

	virtual void Enter(GameObject& ent) = 0;
	virtual void Update(GameObject& ent, double dt) = 0;
	virtual void Exit(GameObject& ent) = 0;
	virtual void ProcessMessage(GameObject* ent, const Message* message) = 0;
private:
};