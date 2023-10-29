#include <OCCEvent.h>

void OCCEvent::AddEvent(std::string name, float weight)
{
	this->eventName = name;
	this->eventWeight = weight;
}

void OCCEvent::SetDesirability(float desirability)
{
	this->desirability = desirability;
}

void OCCEvent::SetProspectRelevance(bool relevance)
{
	this->prospectRelevant = relevance;
}

float OCCEvent::GetDesirability()
{
	return this->desirability;
}

bool OCCEvent::CheckDesirability(OCCEvent event)
{
	if (event.GetDesirability() >= 0)
		return true;

	return false;
}