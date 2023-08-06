#include "ScriptableState.h"

ScriptableState::ScriptableState(sol::function nEnter, sol::function nUpdate, sol::function nExit, sol::function message){
	enterFunc = nEnter;
	updateFunc = nUpdate;
	exitFunc = nExit;
	messageFunc = message;
}

ScriptableState::~ScriptableState()
{
}

void ScriptableState::Enter(GameObject& ent)
{
	try
	{
		NPC& npc = dynamic_cast<NPC&>(ent);
		(enterFunc)(npc);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Scripted State Enter() failed: " << e.what() << std::endl;
	}
}

void ScriptableState::Update(GameObject& ent, double dt)
{
	try
	{
		NPC& npc = dynamic_cast<NPC&>(ent);
		(updateFunc)(npc, dt);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Scripted State Update() failed: " << e.what() << std::endl;
	}
}

void ScriptableState::Exit(GameObject& ent)
{
	try
	{
		NPC& npc = dynamic_cast<NPC&>(ent);
		(exitFunc)(npc);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Scripted State Exit() failed: " << e.what() << std::endl;
	}
}

void ScriptableState::ProcessMessage(GameObject* ent,const Message* message)
{
	try
	{
		NPC& npc = dynamic_cast<NPC&>(*ent);
		(messageFunc)(npc, message);
	}
	catch (const std::exception& e)
	{
		std::cout << "ERROR: Scripted State ProcessMessage() failed: " << e.what() << std::endl;
	}
}


