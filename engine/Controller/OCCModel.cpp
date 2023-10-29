#include <OCCModel.h>

void OCCModel::CalcEmotion(std::string event, float eventStrength, std::string emotion)
{
	if (emotion == "Anger")
	{
		InitAngerFCM(event, eventStrength, emotion);
	}
	else if (emotion == "Fear")
	{
		InitFearFCM(event, eventStrength, emotion);
	}
}

void OCCModel::InitAngerFCM(std::string eventName, float eventStrength, std::string emotion)
{
	FCM attackFCM;
	attackFCM.AddConcept(eventName, eventStrength, 1);
	attackFCM.AddConcept(emotion, 0, 1);

	this->fcm = attackFCM;

	return;
}

void OCCModel::InitFearFCM(std::string eventName, float eventStrength, std::string emotion)
{
	FCM fearFCM;
	fearFCM.AddConcept(eventName, eventStrength, 1);
	fearFCM.AddConcept("Likelihood", 0.8, 1);
	fearFCM.AddConcept(emotion, 0, 1);
	fearFCM.AddRelationship(eventName, "Likelihood", 1);
	fearFCM.AddRelationship("Likelihood", emotion, 1);

	this->fcm = fearFCM;

	return;
}