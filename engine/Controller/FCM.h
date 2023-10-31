#pragma once
#include <iostream>
#include <vector>

struct Concept
{
	std::string name;
	float value;
	float threshold;
};

struct Relationship
{
	std::string concept1;
	std::string concept2;
	float weighting;
};

class FCM
{
public:
	FCM() {};

	void AddConcept(std::string name, float value, float threshold);
	void AddRelationship(std::string concept1, std::string concept2, float weighting);
	Concept GetConcept(std::string conceptName);
	Relationship GetRelationship(std::string concept1, std::string concept2);
	std::vector<Concept> GetConcepts();
	std::vector<Relationship> GetRelationships();
	float GetConceptValue(std::string conceptName);
	float GetRelationshipWeighting(std::string concept1, std::string concept2);
	void SetConceptValue(std::string conceptName, float value);
	void Run();
private:
	std::vector<Concept> concepts;
	std::vector<Relationship> relationships;
};