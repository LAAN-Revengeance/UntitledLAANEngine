#pragma once

#include <FCM.h>
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	void EvaluateAffordance(std::string affordanceName, float distance, Emotion npcEmotion, Personality npcPersonality);
	void CalcEmotion(std::string affordance, float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	void InitAngerFCM(std::string affordance, float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	void InitFearFCM(std::string affordance, float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	float GetEmotionValue(std::string Emotion);
	float GetActionValue();
private:
	FCM fcm;
};