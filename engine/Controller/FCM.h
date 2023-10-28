#pragma once
#include <iostream>
#include <vector>

struct Concept
{
	std::string name;
	float value;
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

	void AddConcept(std::string Name, float Value);
	void AddRelationship(std::string Concept1, std::string Concept2, float Weighting);
	float GetConceptValue(std::string conceptName);
	float GetRelationshipWeighting(std::string concept1, std::string concept2);
	void SetConceptValue(std::string conceptName, float value);
	void Run();
private:
	std::vector<Concept> concepts;
	std::vector<Relationship> relationships;
};