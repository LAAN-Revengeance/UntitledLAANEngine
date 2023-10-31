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
	if (npcEmotion.reactionStrength > 1)
		npcEmotion.reactionStrength = 1;
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

	angerGratitudeGFCM.AddRelationship("Affordance", "Blameworthiness", pow(npcPersonality.GetAgreeablenessPercent(), blameworthiness));
	angerGratitudeGFCM.AddRelationship("Affordance", "Desireability", -0.5);
	angerGratitudeGFCM.AddRelationship("Blameworthiness", emotion, npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship("Desireability", emotion, -npcPersonality.GetNeuroticismPercent());
	angerGratitudeGFCM.AddRelationship(emotion, "Action", npcPersonality.GetConscientiousnessPercent());

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
	fearHopeFCM.AddRelationship("Affordance", "Likelihood", pow(-npcPersonality.GetAgreeablenessPercent(), likelihood));
	fearHopeFCM.AddRelationship("Affordance", "Desireability", 0.5);
	fearHopeFCM.AddRelationship("Likelihood", emotion, npcPersonality.GetNeuroticismPercent());
	fearHopeFCM.AddRelationship("Desireability", emotion, -npcPersonality.GetNeuroticismPercent());
	fearHopeFCM.AddRelationship(emotion, "Action", npcPersonality.GetConscientiousnessPercent());

	return fearHopeFCM;
}

float OCCModel::CalcAffordanceStrength(std::string affordance)
{
	float strength = 0;

	if (affordance == "punch" || affordance == "giveMoney" || affordance == "threaten" || affordance == "generousOffer")
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
	if (affordance == "punch" || affordance == "slap" || affordance == "poke" || affordance == "threaten")
		return false;

	return true;
}

bool OCCModel::CheckProspectRelevant(float distance)
{
	if (distance <= 3)
		return true;
	
	return false;
}