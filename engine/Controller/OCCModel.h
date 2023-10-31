#pragma once

#include <FCM.h>
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	void CalcEmotion(std::string event, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	void InitAngerFCM(std::string eventName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	void InitFearFCM(std::string eventName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality);
	float GetEmotionValue(std::string Emotion);
	float GetActionValue();
private:
	FCM fcm;
};