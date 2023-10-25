#include "AIManager.h"

AIManager& AIManager::Get()
{
	static AIManager a_instance;
	return a_instance;
}

void AIManager::UpdateAgents(double deltaTime)
{
	//run state specific AI only when update times
	accumilator += deltaTime;
	if (accumilator < updateTime) {
		return;
	}

	if (!scene) {
		return;
	}

	msgDispatcher->SendMsgQueue();
	for (auto& it: scene->gameObjects)
	{
		if (it.second) {
			it.second->Update(accumilator);
			it.second->stateMachine.Update(accumilator);
		}
	}
	accumilator = 0;
}

void AIManager::Init(Scene* nScene)
{
	scene = nScene;
	msgDispatcher->SetScene(nScene);
}

void AIManager::AddState(const std::string& stateName, State* nState)
{
	states.insert({stateName,nState});
}

State* AIManager::GetState(const std::string& stateName)
{
	State* rState = states[stateName];

	if (rState != nullptr)
		return rState;

	return nullptr;
}

void AIManager::SendMessage(double delay, int sender, int receiver, int type)
{
	msgDispatcher->SendMessage(delay,sender,receiver,type,nullptr);
}

std::string AIManager::GetStateKey(State* state)
{
	for (auto& it : states) {
		if (state == it.second)
			return it.first;
	}
	return "";
}

AIManager::AIManager()
{
	updateTime = 1.0 / 60.0;
	msgDispatcher = &Dispatcher::Get();
}

AIManager::~AIManager()
{
}
