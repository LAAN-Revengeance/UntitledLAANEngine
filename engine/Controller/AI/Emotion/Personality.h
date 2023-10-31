#pragma once

class Personality
{
public:
	Personality();

	void SetOpenness(float value);
	void SetConscientiousness(float value);
	void SetExtraversion(float value);
	void SetAgreeableness(float value);
	void SetNeuroticism(float value);

	float GetOppenness();
	float GetConscientiousness();
	float GetExtraversion();
	float GetAgreeableness();
	float GetNeuroticism();

	float GetOppennessPercent();
	float GetConscientiousnessPercent();
	float GetExtraversionPercent();
	float GetAgreeablenessPercent();
	float GetNeuroticismPercent();

private:
	float openness;
	float conscientiousness;
	float extraversion;
	float agreeableness;
	float neuroticism;
};