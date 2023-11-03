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

	std::string emotion;
	float affordanceStrength;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {

		npc->SetLastInterracted(_parentObject);
		Personality personality = npc->GetPersonality();

		OCCModel::EvaluateAffordance(GetType(), 0, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		OCCModel::CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), npc->GetPersonality());

		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}

	glm::vec3 knockback = _parentObject->GetForwardVec();
	_otherObject->physicsBody->ApplyForceImpulse(-knockback.x * 10, -knockback.y * 10, -knockback.z * 10);
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