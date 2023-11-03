#include "Affordance_Give_Money.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceGiveMoney::AffordanceGiveMoney(GameObject* go)
{
	_parentObject = go;
	_strength = 1.0;
}

AffordanceGiveMoney::~AffordanceGiveMoney()
{

}

void AffordanceGiveMoney::Activate(GameObject* go)
{
	_otherObject = go;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {
		OCCModel occModel;
		std::string emotion;

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetDescriptor(), GetType(), 5, emotion);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(GetStrength(), emotion, npc->GetEmotion(emotion), personality);

		std::cout << "Emotion = " << emotion << std::endl;
		std::cout << "Affordance Strength = " << GetStrength() << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}
}

void AffordanceGiveMoney::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceGiveMoney::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting money" << std::endl;
	}
}