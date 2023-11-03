#include "FCM.h"

void FCM::AddConcept(std::string name, float value, float threshold)
{
	concepts.push_back({ name, value, threshold });

	return;
}

void FCM::AddRelationship(std::string Concept1, std::string Concept2, float Weighting)
{
	relationships.push_back({ Concept1, Concept2, Weighting });

	return;
}

Concept FCM::GetConcept(std::string conceptName)
{
	for (int i = 0; i < concepts.size(); i++)
	{
		if (concepts[i].name == conceptName)
			return concepts[i];
	}

	std::cout << "Error: No node of name " << conceptName << " was found." << std::endl;

	Concept empty;

	return empty;
}

Relationship FCM::GetRelationship(std::string concept1, std::string concept2)
{
	for (int i = 0; i < relationships.size(); i++)
	{
		if (relationships[i].concept1 == concept1 && relationships[i].concept2 == concept2)
			return relationships[i];
	}

	std::cout << "Error: No relationship between " << concept1 << " and " << concept2 << " was found." << std::endl;

	Relationship Empty;

	return Empty;
}

std::vector<Concept> FCM::GetConcepts()
{
	return this->concepts;
}
std::vector<Relationship> FCM::GetRelationships()
{
	return this->relationships;
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
	bool found = false;
	for (int i = 0; i < concepts.size(); i++)
	{
		if (concepts[i].name == conceptName)
		{
			concepts[i].value = value;
			found = true;
		}
	}

	if (!found)
	{
		std::cout << "Error: concept name " << conceptName << " was not found." << std::endl;
	}
}

void FCM::Run()
{
	for (int i = 0; i < concepts.size(); i++)
	{
		for (int j = 0; j < relationships.size(); j++)
		{
			if (concepts[i].name == relationships[j].concept1)
			{
				Concept concept1 = GetConcept(relationships[j].concept1);
				Concept concept2 = GetConcept(relationships[j].concept2);
				float weighting = GetRelationshipWeighting(relationships[j].concept1, relationships[j].concept2);
				float conceptValue = concept1.value / concept1.threshold;

				if (weighting < 0)
				{
					int num = weighting * 100;
					weighting = num % 100;
					weighting /= 100;
					weighting = 1 + weighting;
				}

				float finalValue = conceptValue * weighting;

				if (finalValue > 1)
					finalValue = 1;
				else if (finalValue < -1)
					finalValue = -1;

				SetConceptValue(relationships[j].concept2, finalValue);
			}
		}
	}
}