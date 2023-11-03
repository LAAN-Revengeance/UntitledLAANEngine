#include "Affordance_Compliment.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceCompliment::AffordanceCompliment(GameObject* go)
{
	_parentObject = go;
}

AffordanceCompliment::~AffordanceCompliment()
{

}

void AffordanceCompliment::Activate(GameObject* go)
{
	_otherObject = go;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {
		OCCModel occModel;
		std::string emotion;
		float affordanceStrength;

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetType(), 0, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), personality);

		std::cout << "Emotion = " << emotion << std::endl;
		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}
}

void AffordanceCompliment::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceCompliment::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting complimented" << std::endl;
	}
}