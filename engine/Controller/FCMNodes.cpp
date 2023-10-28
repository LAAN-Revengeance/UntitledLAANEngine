#include "FCM.h"

void FCMNodes::SetConcept(std::string Name, float Value)
{
	concept.name = Name;
	concept.value = Value;

	return;
}

void FCMNodes::AddRelationship(std::string Concept1, std::string Concept2, float Weighting)
{
	relationships.push_back({ Concept1, Concept2, Weighting });
	return;
}