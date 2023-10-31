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
	if (emotion == "Anger" || emotion == "Gratitude")
	{
		FCM fcm = InitAngerGratitudeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Fear" || emotion == "Hope")
	{
		FCM fcm = InitFearHopeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality);
		fcm.Run();
		npcEmotion.emotionStrength = npcEmotion.emotionStrength + (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength = (fcm.GetConceptValue("Action"));
	}

	if (npcEmotion.emotionStrength > 1)
		npcEmotion.emotionStrength = 1;
	if (npcEmotion.reactionStrength > 1)
		npcEmotion.reactionStrength = 1;
}

FCM OCCModel::InitAngerGratitudeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM attackGratitudeGFCM;
	attackGratitudeGFCM.AddConcept("Affordance", eventStrength, 1);
	attackGratitudeGFCM.AddConcept("Desireability", 0, 1);
	attackGratitudeGFCM.AddConcept("Trust", 0, 1);
	attackGratitudeGFCM.AddConcept("Blameworthiness", 0, 1);
	attackGratitudeGFCM.AddConcept("Irritability", 0, 1);
	attackGratitudeGFCM.AddConcept(emotion, 0, 1);
	attackGratitudeGFCM.AddConcept("Impulsivity", 0, 1);
	attackGratitudeGFCM.AddConcept("Action", 0, 1);
	attackGratitudeGFCM.AddRelationship("Affordance", "Trust", -npcPersonality.GetAgreeablenessPercent());
	attackGratitudeGFCM.AddRelationship("Affordance", "Desireability", -1);
	attackGratitudeGFCM.AddRelationship("Trust", "Blameworthiness", -1);
	attackGratitudeGFCM.AddRelationship("Blameworthiness", "Irritability", 1);
	attackGratitudeGFCM.AddRelationship("Desireability", "Irritability", -1);
	attackGratitudeGFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	attackGratitudeGFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	attackGratitudeGFCM.AddRelationship("Impulsivity", "Action", 1);

	return attackGratitudeGFCM;
}

FCM OCCModel::InitFearHopeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality)
{
	FCM fearHopeFCM;
	fearHopeFCM.AddConcept("Affordance", eventStrength, 1);
	fearHopeFCM.AddConcept("Desireability", 0, 1);
	fearHopeFCM.AddConcept("Trust", 0, 1);
	fearHopeFCM.AddConcept("Likelihood", 0, 1);
	fearHopeFCM.AddConcept("Irritability", 0, 1);
	fearHopeFCM.AddConcept(emotion, 0, 1);
	fearHopeFCM.AddConcept("Impulsivity", 0, 1);
	fearHopeFCM.AddConcept("Action", 0, 1);
	fearHopeFCM.AddRelationship("Affordance", "Trust", npcPersonality.GetAgreeablenessPercent());
	fearHopeFCM.AddRelationship("Affordance", "Desireability", 1);
	fearHopeFCM.AddRelationship("Trust", "Likelihood", -1);
	fearHopeFCM.AddRelationship("Likelihood", "Irritability", 1);
	fearHopeFCM.AddRelationship("Desireability", "Irritability", -1);
	fearHopeFCM.AddRelationship("Irritability", emotion, npcPersonality.GetNeuroticismPercent());
	fearHopeFCM.AddRelationship(emotion, "Impulsivity", npcPersonality.GetConscientiousnessPercent());
	fearHopeFCM.AddRelationship("Impulsivity", "Action", 1);

	return fearHopeFCM;
}

float OCCModel::CalcAffordanceStrength(std::string affordance)
{
	float strength = 0;

	if (affordance == "punch" || affordance == "giveMoney")
	{
		strength = 1;
	}
	else if (affordance == "slap" || affordance == "compliment")
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
	if (distance <= 3)
		return true;
	
	return false;
}