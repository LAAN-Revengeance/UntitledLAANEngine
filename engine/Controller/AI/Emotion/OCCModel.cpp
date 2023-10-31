#include <AI/Emotion/OCCModel.h>

void OCCModel::EvaluateAffordance(std::string affordance, float distance, std::string& emotion, float& affordanceStrength)
{
	bool desirable = CheckDesirable(affordance);
	bool prospectRelevant = CheckProspectRelevant(distance);

	if (!desirable)
	{
		if (prospectRelevant)
			emotion = "Anger";
		else
			emotion = "Fear";
	}
	else
	{
		if (prospectRelevant)
			emotion = "Gratitude";
		else
			emotion = "Hope";
	}

	affordanceStrength = CalcAffordanceStrength(affordance);
}

void OCCModel::CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality)
{
	if (emotion == "Anger")
	{
		FCM fcm = InitAngerFCM(affordanceStrength, emotion, npcEmotion, npcPersonality);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Fear")
	{
		FCM fcm = InitFearFCM(affordanceStrength, emotion, npcEmotion, npcPersonality);
		fcm.Run();
		npcEmotion.emotionStrength = npcEmotion.emotionStrength + (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength = (fcm.GetConceptValue("Action"));
	}

	if (npcEmotion.emotionStrength > 1)
		npcEmotion.emotionStrength = 1;
	if (npcEmotion.reactionStrength > 1)
		npcEmotion.reactionStrength = 1;
}

FCM OCCModel::InitAngerFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	std::cout << npcEmotion.emotionStrength << std::endl;
	FCM attackFCM;
	attackFCM.AddConcept("Affordance", eventStrength, 1);
	attackFCM.AddConcept("Desireability", 0, 1);
	attackFCM.AddConcept("Trust", 0, 1);
	attackFCM.AddConcept("Blameworthiness", 0, 1);
	attackFCM.AddConcept("Irritability", 0, 1);
	attackFCM.AddConcept(emotion, 0, 1);
	attackFCM.AddConcept("Impulsivity", 0, 1);
	attackFCM.AddConcept("Action", 0, 1);
	attackFCM.AddRelationship("Affordance", "Trust", -npcPersonality.GetAgreeablenessPercent());
	attackFCM.AddRelationship("Affordance", "Desireability", -1);
	attackFCM.AddRelationship("Trust", "Blameworthiness", -1);
	attackFCM.AddRelationship("Blameworthiness", "Irritability", 1);
	attackFCM.AddRelationship("Desireability", "Irritability", -1);
	attackFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	attackFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	attackFCM.AddRelationship("Impulsivity", "Action", 1);

	return attackFCM;
}

FCM OCCModel::InitFearFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM fearFCM;
	fearFCM.AddConcept("Affordance", eventStrength, 1);
	fearFCM.AddConcept("Desireability", 0, 1);
	fearFCM.AddConcept("Trust", 0, 1);
	fearFCM.AddConcept("Likelihood", 0, 1);
	fearFCM.AddConcept("Irritability", 0, 1);
	fearFCM.AddConcept(emotion, 0, 1);
	fearFCM.AddConcept("Impulsivity", 0, 1);
	fearFCM.AddConcept("Action", 0, 1);
	fearFCM.AddRelationship("Affordance", "Trust", npcPersonality.GetAgreeablenessPercent());
	fearFCM.AddRelationship("Affordance", "Desireability", 1);
	fearFCM.AddRelationship("Trust", "Likelihood", -1);
	fearFCM.AddRelationship("Likelihood", "Irritability", 1);
	fearFCM.AddRelationship("Desireability", "Irritability", -1);
	fearFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	fearFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	fearFCM.AddRelationship("Impulsivity", "Action", 1);

	return fearFCM;
}

float OCCModel::CalcAffordanceStrength(std::string affordance)
{
	float strength = 0;

	if (affordance == "punch")
	{
		strength = 1;
	}
	else if (affordance == "slap")
	{
		strength = 0.5;
	}
	else if (affordance == "poke")
	{
		strength = 0.2;
	}

	return strength;
}

bool OCCModel::CheckDesirable(std::string affordance)
{
	if (affordance == "punch" || affordance == "slap" || affordance == "poke")
		return false;

	return true;
}

bool OCCModel::CheckProspectRelevant(float distance)
{
	if (distance <= 5)
		return true;
	
	return false;
}