#pragma once
#include "State.h"
#include <sol/sol.hpp>
#include <NPC.h>

class ScriptableState : public State
{
public:
	ScriptableState(sol::function nEnter, sol::function nUpdate, sol::function nExit, sol::function message);
	~ScriptableState();

	void Enter(GameObject& ent);
	void Update(GameObject& ent, double dt);
	void Exit(GameObject& ent);
	void ProcessMessage(GameObject* ent,const Message* message);
private:

	sol::function enterFunc;
	sol::function updateFunc;
	sol::function exitFunc;
	sol::function messageFunc;
};