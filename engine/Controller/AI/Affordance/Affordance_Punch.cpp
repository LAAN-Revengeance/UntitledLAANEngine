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

	OCCModel occModel;
	std::string emotion;
	float affordanceStrength;

	NPC_GameObject* npc = dynamic_cast<NPC_GameObject*>(go);
	if (npc) {

		Personality personality = npc->GetPersonality();

		occModel.EvaluateAffordance(GetType(), 0, emotion, affordanceStrength);
		npc->AddEmotion(emotion);
		occModel.CalcEmotionStrength(affordanceStrength, emotion, npc->GetEmotion(emotion), personality);

		std::cout << "Affordance Strength = " << affordanceStrength << std::endl;
		std::cout << "Emotion Strength = " << npc->GetEmotion(emotion).emotionStrength << std::endl;
		std::cout << "Reaction Strength = " << npc->GetEmotion(emotion).reactionStrength << std::endl;
	}


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