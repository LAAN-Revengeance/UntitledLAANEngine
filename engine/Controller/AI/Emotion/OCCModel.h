#pragma once

#include <FCM.h>
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	std::string EvaluateAffordance(std::string affordance, float distance);
	static void CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality);
	static FCM InitAngerFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	static FCM InitFearFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	bool CheckDesirable(std::string affordance);
	bool CheckProspectRelevant(float distance);
private:
};