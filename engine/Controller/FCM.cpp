#include "FCM.h"

void FCM::AddConcept(std::string name, float value)
{
	concepts.push_back({ name, value });

	return;
}

void FCM::AddRelationship(std::string Concept1, std::string Concept2, float Weighting)
{
	relationships.push_back({ Concept1, Concept2, Weighting });
	return;
}

float FCM::GetConceptValue(std::string conceptName)
{
	for (int i = 0; i < concepts.size(); i++)
	{
		if (concepts[i].name == conceptName)
			return concepts[i].value;
	}

	std::cout << "Error: No node of name " << conceptName << " was found." << std::endl;
	return 0;
}

float FCM::GetRelationshipWeighting(std::string concept1, std::string concept2)
{
	for (int i = 0; i < relationships.size(); i++)
	{
		if (relationships[i].concept1 == concept1 && relationships[i].concept2 == concept2)
			return relationships[i].weighting;
	}

	std::cout << "Error: No relationship between " << concept1 << " and " << concept2 << " was found." << std::endl;

	return 0;
}

void FCM::SetConceptValue(std::string conceptName, float value)
{

}

void FCM::Run()
{
	for (int i = 0; i < concepts.size(); i++)
	{
		for (int j = 0; j < relationships.size(); j++)
		{
			if (concepts[i].name == relationships[i].concept1)
			{
				relationships[i].concept2
			}
		}
	}
}