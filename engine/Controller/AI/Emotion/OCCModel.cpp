#include <AI/Emotion/OCCModel.h>

void OCCModel::CalcEmotion(std::string affordanceName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	if (emotion == "Anger")
	{
		InitAngerFCM(affordanceName, eventStrength, emotion, npcEmotion, npcPersonality);
	}
	else if (emotion == "Fear")
	{
		InitFearFCM(affordanceName, eventStrength, emotion, npcEmotion, npcPersonality);
	}

	fcm.Run();

	npcEmotion.SetEmotionStrength(fcm.GetConceptValue(emotion));
	npcEmotion.SetReactionStrength(fcm.GetConceptValue("Action"));
}

void OCCModel::InitAngerFCM(std::string affordanceName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM attackFCM;
	attackFCM.AddConcept(affordanceName, eventStrength, 1);
	attackFCM.AddConcept("Desireability", 0, 1);
	attackFCM.AddConcept("Trust", 0, 1);
	attackFCM.AddConcept("Blameworthiness", 0, 1);
	attackFCM.AddConcept("Irritability", 0, 1);
	attackFCM.AddConcept(emotion, 0, 1);
	attackFCM.AddConcept("Impulsivity", 0, 1);
	attackFCM.AddConcept("Action", 0, 1);
	attackFCM.AddRelationship(affordanceName, "Trust", -npcPersonality.GetAgreeableness());
	attackFCM.AddRelationship(affordanceName, "Desireability", -1);
	attackFCM.AddRelationship("Trust", "Blameworthiness", -1);
	attackFCM.AddRelationship("Blameworthiness", "Irritability", 2);
	attackFCM.AddRelationship("Desireability", "Irritability", -1);
	attackFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	attackFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	attackFCM.AddRelationship("Impulsivity", "Action", 1);

	this->fcm = attackFCM;

	return;
}

void OCCModel::InitFearFCM(std::string eventName, float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM fearFCM;
	fearFCM.AddConcept(eventName, eventStrength, 1); //Name of the event
	fearFCM.AddConcept("Desireability", 0, 1); //Desireability of the event, either positive or negative
	fearFCM.AddConcept("Trust", 0, 1);
	fearFCM.AddConcept("Likelihood", 0, 1);
	fearFCM.AddConcept("Irritability", 0, 1);
	fearFCM.AddConcept(emotion, 0, 1);
	fearFCM.AddConcept("Impulsivity", 0, 1);
	fearFCM.AddConcept("Action", 0, 1);
	fearFCM.AddRelationship(eventName, "Trust", npcPersonality.GetAgreeableness());
	fearFCM.AddRelationship(eventName, "Desireability", 1);
	fearFCM.AddRelationship("Trust", "Likelihood", -1);
	fearFCM.AddRelationship("Likelihood", "Irritability", 2);
	fearFCM.AddRelationship("Desireability", "Irritability", -1);
	fearFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	fearFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	fearFCM.AddRelationship("Impulsivity", "Action", 1);

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