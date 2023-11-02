#include "state_patrol.h"

State_Patrol::State_Patrol()
{

}

State_Patrol::State_Patrol(std::vector<glm::vec3>& nWaypoints)
{
	waypoints = nWaypoints;
}

State_Patrol::~State_Patrol()
{
}

void State_Patrol::Enter(GameObject& ent)
{
	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;

	if (!npc->HasData("curWaypoint")) {
		GenRandWaypoints(-300, 300, 10);
		npc->AddData("curWaypoint", 0.0f);
	}

	if (!npc->HasData("speed"))
		npc->AddData("speed",40);

	npc->GetDrawItem().Animate("run");

	npc->LookAt(waypoints[(int)npc->GetData("curWaypoint")]);
	npc->MoveTo2D(waypoints[(int)npc->GetData("curWaypoint")], npc->GetData("speed"), 0);
}

void State_Patrol::Update(GameObject& ent, double dt)
{
	NPC* npc = dynamic_cast<NPC*>(&ent);
	if (npc == nullptr)
		return;
	if (!npc->IsTargeting()) {
		if ((int)npc->GetData("curWaypoint") >= (int)waypoints.size() - 1) {
			npc->AddData("curWaypoint", 0.0f);
		}
		else {
			npc->AddData("curWaypoint", npc->GetData("curWaypoint") + 1.0f);
		}
	
		npc->LookAt(waypoints[(int)npc->GetData("curWaypoint")]);
		npc->MoveTo2D(waypoints[(int)npc->GetData("curWaypoint")], npc->GetData("speed"), 0);
	}
	

	//lock to terrain height
	float nY = static_cast<Terrain*>(ResourceManager::Get().GetGameObject("Terrain"))->GetHeight(npc->position.x, npc->position.z) - 1;
	npc->SetPosition({ npc->position.x,nY,npc->position.z });

}

void State_Patrol::Exit(GameObject& ent)
{
}

void State_Patrol::ProcessMessage(GameObject* ent, const Message* message)
{
}

void State_Patrol::GenRandWaypoints(float min, float max, int count)
{
	for (int i = 0; i < count; i++)
	{
		float nx = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		float nz = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		float ny = static_cast<Terrain*>(ResourceManager::Get().GetGameObject("Terrain"))->GetHeight(nx, nz) - 1;
		glm::vec3 nVec = { nx,0,nz };
		waypoints.push_back(nVec);
	}
}
