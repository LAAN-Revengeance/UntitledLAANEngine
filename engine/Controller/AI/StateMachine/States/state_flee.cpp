#include "state_flee.h"

State_Flee::State_Flee()
{
	targetName = "Player";
	target = nullptr;
}

State_Flee::State_Flee(std::string& nTarget)
{
	targetName = nTarget;
	target = nullptr;
}

State_Flee::~State_Flee()
{
}

void State_Flee::Enter(GameObject& ent)
{
	if(!target)
		target = ResourceManager::Get().GetGameObject(targetName);

	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;

	npc->GetDrawItem().Animate("run");
}

void State_Flee::Update(GameObject& ent, double dt)
{
	if (!target) {
		target = ResourceManager::Get().GetGameObject(targetName);
		if (!target)
			return;
	}

	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;

	glm::vec3 toTarget = target->position - npc->position;

	glm::vec3 nPos = npc->position - toTarget;
	npc->LookAt(nPos);
	npc->MoveTo3D(nPos, 20, 0);

	//lock to terrain height
	float nY = static_cast<Terrain*>(ResourceManager::Get().GetGameObject("Terrain"))->GetHeight(npc->position.x, npc->position.z) - 1;
	npc->SetPosition({ npc->position.x,nY,npc->position.z });

}

void State_Flee::Exit(GameObject& ent)
{
}

void State_Flee::ProcessMessage(GameObject* ent, const Message* message)
{
}
