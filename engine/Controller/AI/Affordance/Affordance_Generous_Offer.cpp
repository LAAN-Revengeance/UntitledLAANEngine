#include "Affordance_Generous_Offer.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceGenerousOffer::AffordanceGenerousOffer(GameObject* go)
{
	_parentObject = go;
}

AffordanceGenerousOffer::~AffordanceGenerousOffer()
{

}

void AffordanceGenerousOffer::Activate(GameObject* go)
{
	_otherObject = go;

	NPC_GameObject* npc = dynamic_cast<NPC_GameObject*>(go);
	if (npc) {
		OCCModel occModel;
		std::string emotion;
		float affordanceStrength;

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetType(), 5, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), personality);

		std::cout << "Emotion = " << emotion << std::endl;
		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}
}

void AffordanceGenerousOffer::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceGenerousOffer::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting threatened" << std::endl;
	}
}