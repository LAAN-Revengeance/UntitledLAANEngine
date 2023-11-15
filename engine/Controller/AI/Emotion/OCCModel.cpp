#include <AI/Emotion/OCCModel.h>

void OCCModel::EvaluateAffordance(std::string affordance, std::string affordanceType, bool affordanceDanger, std::string& emotion)
{
	bool desirable = !affordanceDanger;
	bool prospectRelevant = CheckProspectRelevant(affordanceType);

	if (!desirable)
	{
		if (prospectRelevant)
			emotion = "Fear";
		else
			emotion = "Anger";
	}
	else
	{
		if (prospectRelevant)
			emotion = "Hope";
		else
			emotion = "Gratitude";
	}
}

void OCCModel::CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality)
{
	if (emotion == "Anger")
	{
		FCM fcm = InitAngerGratitudeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, false, false);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Gratitude")
	{
		FCM fcm = InitAngerGratitudeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, true, true);
		fcm.Run();
		npcEmotion.emotionStrength += (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength += (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Fear")
	{
		FCM fcm = InitFearHopeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, false, false);
		fcm.Run();
		npcEmotion.emotionStrength = npcEmotion.emotionStrength + (fcm.GetConceptValue(emotion));
		npcEmotion.reactionStrength = (fcm.GetConceptValue("Action"));
	}
	else if (emotion == "Hope")
	{
		FCM fcm = InitFearHopeFCM(affordanceStrength, emotion, npcEmotion, npcPersonality, true, true);
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

FCM OCCModel::InitAngerGratitudeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirable, bool blameworthy)
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

	if(blameworthy)
		angerGratitudeGFCM.AddRelationship("Affordance", "Blameworthiness", -npcPersonality.GetAgreeablenessPercent());
	else
		angerGratitudeGFCM.AddRelationship("Affordance", "Blameworthiness", npcPersonality.GetAgreeablenessPercent());

	if(desirable)
		angerGratitudeGFCM.AddRelationship("Affordance", "Desireability", 0.5);
	else
		angerGratitudeGFCM.AddRelationship("Affordance", "Desireability", -0.5);

	angerGratitudeGFCM.AddRelationship("Blameworthiness", emotion, npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship("Desireability", emotion, npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship(emotion, "Action", -npcPersonality.GetConscientiousnessPercent());

	return angerGratitudeGFCM;
}

FCM OCCModel::InitFearHopeFCM(float eventStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirable, bool likely)
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

	if(likely)
		fearHopeFCM.AddRelationship("Affordance", "Likelihood", -npcPersonality.GetAgreeablenessPercent());
	else
		fearHopeFCM.AddRelationship("Affordance", "Likelihood", npcPersonality.GetAgreeablenessPercent());

	if(desirable)
		fearHopeFCM.AddRelationship("Affordance", "Desireability", 0.5);
	else
		fearHopeFCM.AddRelationship("Affordance", "Desireability", -0.5);

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
		return false;
	
	return true;
}