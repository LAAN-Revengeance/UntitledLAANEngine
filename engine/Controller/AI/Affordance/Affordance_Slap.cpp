#include "Affordance_Slap.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceSlap::AffordanceSlap(GameObject* go)
{
	_parentObject = go;
	_strength = 0.5;
	_dangerous = true;
}

AffordanceSlap::~AffordanceSlap()
{

}

void AffordanceSlap::Activate(GameObject* go)
{
	_otherObject = go;
	std::string emotion;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {
		npc->SetLastInterracted(_parentObject);
		OCCModel::EvaluateAffordance(GetDescriptor(), GetType(), GetDanger(), emotion);
		npc->AddEmotion(emotion);
		OCCModel::CalcEmotionStrength(GetStrength(), emotion, npc->GetEmotion(emotion), npc->GetPersonality());

		std::cout << "Affordance Strength = " << GetStrength() << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}

	glm::vec3 knockback = _parentObject->GetForwardVec();
	_otherObject->physicsBody->ApplyForceImpulse(-knockback.x * 2, -knockback.y * 2, -knockback.z * 2);
}

void AffordanceSlap::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceSlap::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting slapped" << std::endl;
	}
}