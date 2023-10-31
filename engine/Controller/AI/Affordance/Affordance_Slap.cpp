#include "Affordance_Slap.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceSlap::AffordanceSlap(GameObject* go)
{
	_parentObject = go;
}

AffordanceSlap::~AffordanceSlap()
{

}

void AffordanceSlap::Activate(GameObject* go)
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
	_otherObject->physicsBody->ApplyForceImpulse(-knockback.x * 2, -knockback.y * 2, -knockback.z * 2);
	
	if (npc->GetEmotion(emotion).reactionStrength == 1)
	{
		_parentObject->physicsBody->ApplyForceImpulse(knockback.x * 2, knockback.y * 2, knockback.z * 2);
		npc->SetEmotionStrength(emotion, 0);
		npc->SetReactionStrength(emotion, 0);
	}
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