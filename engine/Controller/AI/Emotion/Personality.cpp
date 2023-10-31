#include <AI/Emotion/Personality.h>

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
	if (value > 120)
		value = 120;
	else if (value < 0)
		value = 0;

	this->openness = value;
}

void Personality::SetConscientiousness(float value)
{
	if (value > 120)
		value = 120;
	else if (value < 0)
		value = 0;

	this->conscientiousness = value;
}

void Personality::SetExtraversion(float value)
{
	if (value > 120)
		value = 120;
	else if (value < 0)
		value = 0;

	this->extraversion = value;
}

void Personality::SetAgreeableness(float value)
{
	if (value > 120)
		value = 120;
	else if (value < 0)
		value = 0;

	this->agreeableness = value;
}

void Personality::SetNeuroticism(float value)
{
	if (value > 120)
		value = 120;
	else if (value < 0)
		value = 0;

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

float Personality::GetOppennessPercent()
{
	return (this->openness / 60);
}

float Personality::GetConscientiousnessPercent()
{
	return (this->conscientiousness / 60);
}

float Personality::GetExtraversionPercent()
{
	return (this->extraversion / 60);
}

float Personality::GetAgreeablenessPercent()
{
	return (this->agreeableness / 60);
}

float Personality::GetNeuroticismPercent()
{
	return (this->neuroticism / 60);
}