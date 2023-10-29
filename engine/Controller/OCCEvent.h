#pragma once
#include <iostream>

class OCCEvent
{
public:
	OCCEvent() {};

	void AddEvent(std::string name, float weight);
	void SetDesirability(float desirability);
	void SetProspectRelevance(bool relevance);

	float GetDesirability();
	bool CheckDesirability(OCCEvent event);

private:
	std::string eventName;
	float eventWeight;
	float eventLikelihood;
	float desirability;
	bool prospectRelevant;
};