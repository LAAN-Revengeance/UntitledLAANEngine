#pragma once
#include <iostream>

/**
*	@Class Emotion
*	@brief The Emotion class stores the emotion, its strength, and a
*	reaction strength
*
*	@author Aidan O'Connor
*	@version 1.0
*	@date 29/10/2023
*/

class Emotion
{
public:
	/// Stores emotion name as a string
	std::string emotion = "";

	/// Stores emotion strength as a float
	float emotionStrength = 0;

	/// Stores reaction strength as a float
	float reactionStrength = 0;

	/**
	 * @brief Default emotion constructor
	*/
	Emotion();

	/**
	 * @brief Emotion constructor for instances where name is known
	 * @param name
	*/
	Emotion(std::string name);

	/**
	 * @brief Emotion constructor for instances where name and strength is known
	 * @param name, emotionStrength
	*/
	Emotion(std::string name, float emotionStrength);

	/**
	 * @brief Emotion constructor for instances where name, strength, and reaction strength are known
	 * @param name, emotionStrength, reactionStrength
	*/
	Emotion(std::string name, float emotionStrength, float reactionStrength);
};