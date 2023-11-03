#pragma once

#include "FCM.h"
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	static void EvaluateAffordance(std::string affordance, std::string affordanceType, bool affordanceDanger, std::string& emotion);
	static void CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality);
	static FCM InitAngerGratitudeFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirability, bool blameworthiness);
	static FCM InitFearHopeFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirability, bool likelihood);
	static bool CheckDesirable(std::string affordance);
	static bool CheckProspectRelevant(std::string affordanceType);
private:
};