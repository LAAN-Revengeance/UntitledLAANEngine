#include "Affordance_Poke.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordancePoke::AffordancePoke(GameObject* go)
{
	_parentObject = go;
}

AffordancePoke::~AffordancePoke()
{

}

void AffordancePoke::Activate(GameObject* go)
{
	_otherObject = go;
	std::string emotion;

	NPC_GameObject* npc = dynamic_cast<NPC_GameObject*>(go);
	if (npc) {
		OCCModel occModel;
		float affordanceStrength;

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetType(), 0, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), npc->GetPersonality());

		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}

	glm::vec3 knockback = _parentObject->GetForwardVec();
	_otherObject->physicsBody->ApplyForceImpulse(-knockback.x * 0.1, -knockback.y * 0.1, -knockback.z * 0.1);

	if (npc->GetEmotion(emotion).reactionStrength == 1.0f)
	{
		glm::vec3 playerKnockback = _otherObject->GetForwardVec();
		_parentObject->physicsBody->ApplyForceImpulse(knockback.x * 10, knockback.y * 10, -knockback.z * 10);
		npc->SetEmotionStrength(emotion, 0);
		npc->SetReactionStrength(emotion, 0);
	}
}

void AffordancePoke::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordancePoke::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting poked" << std::endl;
	}
}