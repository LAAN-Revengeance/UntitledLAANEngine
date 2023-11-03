#pragma once

/**
*	@Class Personality
*	@brief The Personality class stores personality according to
*	a 120 question test regarding the big 5 factors of personality
*	and provides functions to set and get the values as well as the
*	value as a percentage
*
*	@author Aidan O'Connor
*	@version 1.0
*	@date 29/10/2023
*/

class Personality
{
public:
	/**
	 * @brief Default personality cosntructor sets all factors to 60, an average value
	*/
	Personality();

	/**
	 * @brief Setter for the openness variable
	 * @param value
	*/
	void SetOpenness(float value);

	/**
	 * @brief Setter for the conscientiousness variable
	 * @param value
	*/
	void SetConscientiousness(float value);

	/**
	 * @brief Setter for the extraversion variable
	 * @param value
	*/
	void  SetExtraversion(float value);

	/**
	 * @brief Setter for the agreeableness variable
	 * @param value
	*/
	void  SetAgreeableness(float value);

	/**
	 * @brief Setter for the neuroticism variable
	 * @param value
	*/
	void  SetNeuroticism(float value);

	/**
	 * @brief Getter for the openness variable
	 * @return openness
	*/
	float GetOpenness();

	/**
	 * @brief Getter for the conscientiousness variable
	 * @return conscientiousness
	*/
	float GetConscientiousness();

	/**
	 * @brief Getter for the extraversion variable
	 * @return extraversion
	*/
	float GetExtraversion();

	/**
	 * @brief Getter for the agreeableness variable
	 * @return agreeableness
	*/
	float GetAgreeableness();

	/**
	 * @brief Getter for the neuroticism variable
	 * @return neuroticism
	*/
	float GetNeuroticism();

	/**
	 * @brief Getter for the oppenness variable as a percentage
	 * @return oppenness
	*/
	float GetOppennessPercent();

	/**
	 * @brief Getter for the conscientiousness variable as a percentage
	 * @return conscientiousness
	*/
	float GetConscientiousnessPercent();

	/**
	 * @brief Getter for the extraversion variable as a percentage
	 * @return extraversion
	*/
	float GetExtraversionPercent();

	/**
	 * @brief Getter for the agreeableness variable as a percentage
	 * @return agreeableness
	*/
	float GetAgreeablenessPercent();

	/**
	 * @brief Getter for the neuroticism variable as a percentage
	 * @return neuroticism
	*/
	float GetNeuroticismPercent();
private:
	/// Stores the value of openness as a float
	float openness;

	/// Stores the value of conscientiousness as a float
	float conscientiousness;

	/// Stores the value of extraversion as a float
	float extraversion;

	/// Stores the value of agreeableness as a float
	float agreeableness;

	/// Stores the value of neuroticism as a float
	float neuroticism;
};