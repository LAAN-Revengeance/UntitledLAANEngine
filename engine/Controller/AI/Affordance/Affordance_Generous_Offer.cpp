#include "Affordance_Generous_Offer.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceGenerousOffer::AffordanceGenerousOffer(GameObject* go)
{
	_parentObject = go;
	_strength = 1.0;
	_dangerous = false;
}

AffordanceGenerousOffer::~AffordanceGenerousOffer()
{

}

void AffordanceGenerousOffer::Activate(GameObject* go)
{
	_otherObject = go;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {
		std::string emotion;

		OCCModel::EvaluateAffordance(GetDescriptor(), GetType(), GetDanger(), emotion);
		npc->AddEmotion(emotion);
		OCCModel::CalcEmotionStrength(GetStrength(), emotion, npc->GetEmotion(emotion), npc->GetPersonality());

		std::cout << "Emotion = " << emotion << std::endl;
		std::cout << "Affordance Strength = " << GetStrength() << std::endl;
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