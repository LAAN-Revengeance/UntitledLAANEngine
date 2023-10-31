#include "Affordance_Punch.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordancePunch::AffordancePunch(GameObject* go)
{
	_parentObject = go;
}

AffordancePunch::~AffordancePunch()
{

}

void AffordancePunch::Activate(GameObject* go)
{
	_otherObject = go;

	NPC_GameObject* npc = dynamic_cast<NPC_GameObject*>(go);
	if (npc) {
		
	}

}

void AffordancePunch::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordancePunch::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting punched" << std::endl;
	}
}