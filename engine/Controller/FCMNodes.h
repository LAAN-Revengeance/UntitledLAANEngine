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

class FCMNodes
{
public:
	FCMNodes() {};

	void SetConcept(std::string Name, float Value);
	void AddRelationship(std::string Concept1, std::string Concept2, float Weighting);
private:
	Concept concept;
	std::vector<Relationship> relationships;
};