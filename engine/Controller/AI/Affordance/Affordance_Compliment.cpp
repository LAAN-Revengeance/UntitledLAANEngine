#include "Affordance_Compliment.h"
#include <GameObject.h>
#include <GameObjects/NPC_GameObject.h>
#include <AI/Emotion/OCCModel.h>

AffordanceCompliment::AffordanceCompliment(GameObject* go)
{
	_parentObject = go;
	_strength = 0.5;
	_dangerous = false;
}

AffordanceCompliment::~AffordanceCompliment()
{

}

void AffordanceCompliment::Activate(GameObject* go)
{
	_otherObject = go;

	NPC* npc = dynamic_cast<NPC*>(go);
	if (npc) {
		npc->SetLastInterracted(_parentObject);
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