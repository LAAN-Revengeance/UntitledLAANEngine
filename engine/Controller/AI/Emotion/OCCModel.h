#pragma once

#include <FCM.h>
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	static void EvaluateAffordance(std::string affordance, float distance, std::string& emotion, float& affordanceStrength);
	static void CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality);
	static FCM InitAngerFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	static FCM InitFearFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	static float CalcAffordanceStrength(std::string affordance);
	static bool CheckDesirable(std::string affordance);
	static bool CheckProspectRelevant(float distance);
private:
};