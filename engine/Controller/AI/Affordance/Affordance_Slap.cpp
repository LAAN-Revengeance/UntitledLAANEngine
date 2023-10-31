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