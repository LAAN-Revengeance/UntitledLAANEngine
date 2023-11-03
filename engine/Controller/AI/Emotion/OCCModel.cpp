#include <AI/Emotion/OCCModel.h>

void OCCModel::EvaluateAffordance(std::string affordance, std::string affordanceType, float distance, std::string& emotion)
{
	bool desirable = CheckDesirable(affordance);
	bool prospectRelevant = CheckProspectRelevant(affordanceType);

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
}

void OCCModel::CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality)
{
	if (emotion == "Anger")
	{
		FCM fcm = InitAngerGratitudeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, 2);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Gratitude")
	{
		FCM fcm = InitAngerGratitudeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, 4);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Fear")
	{
		FCM fcm = InitFearHopeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, 2);
		fcm.Run();
		npcEmotion.emotionStrength = npcEmotion.emotionStrength + (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength = (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Hope")
	{
		FCM fcm = InitFearHopeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, 4);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}

	if (npcEmotion.emotionStrength > 1)
		npcEmotion.emotionStrength = 1;
	else if (npcEmotion.emotionStrength < 0)
		npcEmotion.emotionStrength = 0;

	if (npcEmotion.reactionStrength > 1)
		npcEmotion.reactionStrength = 1;
	else if (npcEmotion.reactionStrength < 0)
		npcEmotion.reactionStrength = 0;
}

FCM OCCModel::InitAngerGratitudeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, int blameworthiness)
{
	FCM angerGratitudeGFCM;
	angerGratitudeGFCM.AddConcept("Affordance", eventStrength, 1);
	angerGratitudeGFCM.AddConcept("Desireability", 0, 1);
	angerGratitudeGFCM.AddConcept("Trust", 0, 1);
	angerGratitudeGFCM.AddConcept("Blameworthiness", 0, 1);
	angerGratitudeGFCM.AddConcept("Irritability", 0, 1);
	angerGratitudeGFCM.AddConcept(emotion, 0, 1);
	angerGratitudeGFCM.AddConcept("Impulsivity", 0, 1);
	angerGratitudeGFCM.AddConcept("Action", 0, 1);

	angerGratitudeGFCM.AddRelationship("Affordance", "Blameworthiness", -npcPersonality.GetAgreeablenessPercent());
	angerGratitudeGFCM.AddRelationship("Affordance", "Desireability", -0.5);
	angerGratitudeGFCM.AddRelationship("Blameworthiness", emotion, npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship("Desireability", emotion, npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship(emotion, "Action", -npcPersonality.GetConscientiousnessPercent());

	return angerGratitudeGFCM;
}

FCM OCCModel::InitFearHopeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, int likelihood)
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
	fearHopeFCM.AddRelationship("Affordance", "Likelihood", -npcPersonality.GetAgreeablenessPercent());
	fearHopeFCM.AddRelationship("Affordance", "Desireability", 0.5);
	fearHopeFCM.AddRelationship("Likelihood", emotion, npcPersonality.GetNeuroticismPercent());
	fearHopeFCM.AddRelationship("Desireability", emotion, npcPersonality.GetNeuroticismPercent());
	fearHopeFCM.AddRelationship(emotion, "Action", -npcPersonality.GetConscientiousnessPercent());

	return fearHopeFCM;
}

bool OCCModel::CheckDesirable(std::string affordance)
{
	if (affordance == "punch" || affordance == "slap" || affordance == "poke" || affordance == "threaten")
		return false;

	return true;
}

bool OCCModel::CheckProspectRelevant(std::string affordanceType)
{
	if (affordanceType == "agent")
		return true;
	
	return false;
}