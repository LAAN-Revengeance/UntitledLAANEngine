#include "Affordance_Threaten.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceThreaten::AffordanceThreaten(GameObject* go)
{
	_parentObject = go;
	_strength = 1.0;
	_dangerous = true;
}

AffordanceThreaten::~AffordanceThreaten()
{

}

void AffordanceThreaten::Activate(GameObject* go)
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