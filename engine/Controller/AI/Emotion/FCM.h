#pragma once
#include <iostream>
#include <vector>

/**
*	@Class FCM
*	@brief The FCM class gives the ability to create a node, relationshop, weighting
*	system that runs with the default formula and grants the ability to get node values
*
*	@author Aidan O'Connor
*	@version 1.0
*	@date 28/10/2023
*/

/// Data type for storing concept node related data
struct Concept
{
	/// Concept name as a string
	std::string name;

	/// Concept value as a float
	float value;

	/// Max concept value as a float
	float threshold;
};

/// Data type for storing relationship related data
struct Relationship
{
	/// Source concept as a string
	std::string concept1;

	/// Destination concept as a string
	std::string concept2;

	/// Relationship weighting as a float
	float weighting;
};

class FCM
{
public:

	/**
	 * @brief Default FCM constructor
	*/
	FCM() {};

	/**
	 * @brief Sets the values for a new Concept data type
	 * @param name, value, threshold
	*/
	void AddConcept(std::string name, float value, float threshold);

	/**
	 * @brief Sets the values for a new Relationship data type
	 * @param concept1, concept2, weighting
	*/
	void AddRelationship(std::string concept1, std::string concept2, float weighting);

	/**
	 * @brief Gets the values of a concept with a given name
	 * @param conceptName
	 * @return concept
	*/
	Concept GetConcept(std::string conceptName);

	/**
	 * @brief Gets the values of a relationship with the given names
	 * @param concept1, concept2
	 * @return relationship
	*/
	Relationship GetRelationship(std::string concept1, std::string concept2);

	/**
	 * @brief Return all concepts held in the FCM
	 * @return concepts
	*/
	std::vector<Concept> GetConcepts();

	/**
	 * @brief Return all relationships held in the FCM
	 * @return relationships
	*/
	std::vector<Relationship> GetRelationships();

	/**
	 * @brief Return value of a concept with the given name
	 * @param conceptName
	 * @return value
	*/
	float GetConceptValue(std::string conceptName);

	/**
	 * @brief Return weighting of the relationship between two concepts
	 * @param concept1, concept2
	 * @return weighting
	*/
	float GetRelationshipWeighting(std::string concept1, std::string concept2);

	/**
	 * @brief Sets the value of a concept
	 * @param conceptName, value
	*/
	void SetConceptValue(std::string conceptName, float value);

	/**
	 * @brief Runs the fcm by calculating values of each node from given relationships
	*/
	void Run();
private:
	/// Stores all concepts of an FCM
	std::vector<Concept> concepts;

	/// Stores all relationships of an FCM
	std::vector<Relationship> relationships;
};