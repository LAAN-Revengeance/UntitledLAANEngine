#pragma once
#include "FCMNodes.h"

class FCM
{
public:
	FCM() {};
	void AddNode(std::string concept, float conceptValue, std::string relationshipNode1, std::string relationshipNode2, float relationshipWeighting);
	void AddConcept(std::string concept, float value);
	void AddRelationship(std::string concpept1, std::string concept2, float weighting);
private:
	std::vector<FCMNodes> nodes;
};