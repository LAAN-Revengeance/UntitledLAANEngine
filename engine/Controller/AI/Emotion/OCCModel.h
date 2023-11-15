#pragma once

#include "FCM.h"
#include <GameObjects/NPC_GameObject.h>

/**
*	@Class OCCModel
*	@brief The OCCModel class allows the user to determine the emotion
*	an agent should be feeling based off of affordances and uses an FCM
*	to determine feeling and reaction strength
*
*	@author Aidan O'Connor
*	@version 1.0
*	@date 31/10/2023
*/

class OCCModel
{
public:
	/**
	 * @brief Takes affordance data to determine an agents emotion
	 * @param affordance, affordanceType, affordanceDanger, emotion
	*/
	static void EvaluateAffordance(std::string affordance, std::string affordanceType, bool affordanceDanger, std::string& emotion);

	/**
	 * @brief Takes affordance and NPC data to determine emotion and reaction intensities
	 * @param affordanceStrength, emotion, npcEmotion, npcPersonality
	*/
	static void CalcEmotionStrength(float affordanceStrength, std::string emotion, Emotion& npcEmotion, Personality npcPersonality);

	/**
	 * @brief Takes node and weight related data to create an FCM for determining anger or gratitude intensities
	 * @param affordanceStrength, emotion, npcEmotion, npcPersonality, desirability, blameworthiness
	 * @return angerGratitudeFCM
	*/
	static FCM InitAngerGratitudeFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirability, bool blameworthiness);

	/**
	 * @brief Takes node and weight related data to create an FCM for determining fear or hope intensities
	 * @param affordanceStrength, emotion, npcEmotion, npcPersonality, desirability, likelihood
	 * @return fearHopeFCM
	*/
	static FCM InitFearHopeFCM(float affordanceStrength, std::string emotion, Emotion npcEmotion, Personality npcPersonality, bool desirability, bool likelihood);

	/**
	 * @brief Checks if an affordance's prospect is relevant
	 * @param affordanceType
	 * @return prospectRelevance
	*/
	static bool CheckProspectRelevant(std::string affordanceType);
private:
};