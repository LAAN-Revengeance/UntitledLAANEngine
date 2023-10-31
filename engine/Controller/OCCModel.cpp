#include <OCCModel.h>

void OCCModel::CalcEmotion(std::string event, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	if (emotion == "Anger")
	{
		InitAngerFCM(event, eventStrength, emotion, npcEmotion, npcPersonality);
	}
	else if (emotion == "Fear")
	{
		InitFearFCM(event, eventStrength, emotion, npcEmotion, npcPersonality);
	}

	fcm.Run();

	npcEmotion.SetEmotionStrength(fcm.GetConceptValue(emotion));
	npcEmotion.SetReactionStrength(fcm.GetConceptValue("Action"));
}

void OCCModel::InitAngerFCM(std::string eventName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM attackFCM;
	attackFCM.AddConcept(eventName, eventStrength, 1);
	attackFCM.AddConcept("Desireability", 0, 1);
	attackFCM.AddConcept("Trust", 0, 1);
	attackFCM.AddConcept("Blameworthiness", 0, 1);
	attackFCM.AddConcept("Irritability", 0, 1);
	attackFCM.AddConcept(emotion, 0, 1);
	attackFCM.AddConcept("Impulsivity", 0, 1);
	attackFCM.AddConcept("Action", 0, 1);
	attackFCM.AddRelationship(eventName, "Trust", npcPersonality.GetAgreeableness());
	attackFCM.AddRelationship("Trust", "Blameworthiness", 1);
	attackFCM.AddRelationship(eventName, "Desireability", -1);
	attackFCM.AddRelationship("Blameworthiness", "Irritability", 2);
	attackFCM.AddRelationship("Desireability", "Irritability", -1);
	attackFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	attackFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	attackFCM.AddRelationship(emotion, "Action", 1);

	this->fcm = attackFCM;

	return;
}

void OCCModel::InitFearFCM(std::string eventName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM fearFCM;
	fearFCM.AddConcept(eventName, eventStrength, 1);
	fearFCM.AddConcept("Desireability", 0, 1);
	fearFCM.AddConcept("Likelihood", 0, 1);
	fearFCM.AddConcept("Fear", 0, 1);
	fearFCM.AddConcept("Action", 0, 1);
	fearFCM.AddRelationship(eventName, "Trust", npcPersonality.GetAgreeableness());
	fearFCM.AddRelationship("Trust", "Likelihood", 1);
	fearFCM.AddRelationship(eventName, "Desireability", -1);
	fearFCM.AddRelationship("Likelihood", "Irritability", 2);
	fearFCM.AddRelationship("Desireability", "Irritability", -1);
	fearFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	fearFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	fearFCM.AddRelationship(emotion, "Action", 1);

	this->fcm = fearFCM;

	return;
}

float OCCModel::GetEmotionValue(std::string Emotion)
{
	return this->fcm.GetConceptValue(Emotion);
}

float OCCModel::GetActionValue()
{
	return this->fcm.GetConceptValue("Action");
}