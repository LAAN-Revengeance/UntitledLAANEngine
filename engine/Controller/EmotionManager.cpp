#include "EmotionManger.h"

void EmotionManager::InitFCM()
{
	FCM fcm;
	fcm.AddConcept("Attack", 1, 1);
	fcm.AddConcept("Anger", 0, 1);
	fcm.AddRelationship("Attack", "Anger", 1);

	fcms.push_back(fcm);

	return;
}

void EmotionManager::RunFCM(FCM fcm)
{
	fcm.Run();

	std::cout << fcm.GetConceptValue("Anger") << std::endl;

	return;
}

FCM EmotionManager::GetFCM()
{
	return fcms[0];
}