#include "Affordance_Threaten.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceThreaten::AffordanceThreaten(GameObject* go)
{
	_parentObject = go;
}

AffordanceThreaten::~AffordanceThreaten()
{

}

void AffordanceThreaten::Activate(GameObject* go)
{
	_otherObject = go;

	NPC* npc = dynamic_cast<NPC*>(go);
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

void AffordanceThreaten::Deactivate()
{
	_active = false;
	_otherObject = nullptr;
}

void AffordanceThreaten::Update(double deltaTime)
{
	if (_active) {
		std::cout << "Yall getting threatened" << std::endl;
	}
}