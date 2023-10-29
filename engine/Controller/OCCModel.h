#pragma once

#include <FCM.h>
#include <GameObjects/NPC_GameObject.h>

class OCCModel
{
public:
	void CalcEmotion(std::string event, float eventStrength, std::string emotion);
	void InitAngerFCM(std::string eventName, float eventStrength, std::string emotion);
	void InitFearFCM(std::string eventName, float eventStrength, std::string emotion);
	float GetEmotionValue(std::string Emotion);
	float GetActionValue();
private:
	FCM fcm;
};