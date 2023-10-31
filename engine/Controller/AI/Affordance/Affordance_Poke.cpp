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

	NPC_GameObject* npc = dynamic_cast<NPC_GameObject*>(go);
	if (npc) {
		OCCModel occModel;
		std::string emotion;
		float affordanceStrength;

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetType(), 0, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), personality);

		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
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