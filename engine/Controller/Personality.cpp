#include <Personality.h>

Personality::Personality()
{
	openness = 60;
	conscientiousness = 60;
	extraversion = 60;
	agreeableness = 60;
	neuroticism = 60;
}

void Personality::SetOpenness(float value)
{
	this->openness = value;
}

void Personality::SetConscientiousness(float value)
{
	this->conscientiousness = value;
}

void Personality::SetExtraversion(float value)
{
	this->extraversion = value;
}

void Personality::SetAgreeableness(float value)
{
	this->agreeableness = value;
}

void Personality::SetNeuroticism(float value)
{
	this->neuroticism = value;
}

float Personality::GetOppenness()
{
	return this->openness;
}

float Personality::GetConscientiousness()
{
	return this->conscientiousness;
}

float Personality::GetExtraversion()
{
	return this->extraversion;
}

float Personality::GetAgreeableness()
{
	return this->agreeableness;
}

float Personality::GetNeuroticism()
{
	return this->neuroticism;
}