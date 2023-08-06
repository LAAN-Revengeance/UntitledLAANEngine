#include "state_evade.h"

State_Evade::State_Evade()
{
	targetName = "Player";
	target = nullptr;
}

State_Evade::State_Evade(std::string& nTargetName)
{
	target = nullptr;
	targetName = nTargetName;
}

State_Evade::~State_Evade()
{
}

void State_Evade::Enter(GameObject& ent)
{
	if (!target)
		target = ResourceManager::Get().GetGameObject(targetName);
	if (!stateWander)
		stateWander = AIManager::Get().GetState("state_wander");

	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;

	if (!npc->HasData("fleeSpeed"))
		npc->AddData("fleeSpeed", 20);

	if (!npc->HasData("fleeDistance"))
		npc->AddData("fleeDistance", 300);

	npc->GetDrawItem().Animate("run");
}

void State_Evade::Update(GameObject& ent, double dt)
{

	if (!target) {
		target = ResourceManager::Get().GetGameObject(targetName);
		if (!target)
			return;
	}

	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;

	
	glm::vec3 predictedPos = target->position + target->rigidBody.GetLinearVelocty() * 2.0f;

	glm::vec3 toTarget =  predictedPos - npc->position;

	glm::vec3 nPos = npc->position - glm::normalize(toTarget);
	npc->LookAt(nPos);
	npc->MoveTo3D(nPos, npc->GetData("fleeSpeed"), 0);


	//lock to terrain height
	float nY = static_cast<Terrain*>(ResourceManager::Get().GetGameObject("Terrain"))->GetHeight(npc->position.x, npc->position.z) - 1;
	npc->SetPosition({ npc->position.x,nY,npc->position.z });

	if (glm::length(toTarget) > npc->GetData("fleeDistance")) {
		npc->stateMachine.ChangeState(*stateWander);
	}
}

void State_Evade::Exit(GameObject& ent)
{
}

void State_Evade::ProcessMessage(GameObject* ent, const Message* message)
{
}