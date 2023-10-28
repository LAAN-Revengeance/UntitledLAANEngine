#pragma once
#include "FCM.h"

class EmotionManager
{
public:
	EmotionManager() {};

	void InitFCM();
	void RunFCM(FCM fcm);
	FCM GetFCM();
private:
	std::vector<FCM> fcms;
};