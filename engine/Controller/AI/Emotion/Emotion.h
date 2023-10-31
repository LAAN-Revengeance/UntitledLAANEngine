#pragma once
#include <iostream>

class Emotion
{
public:
	std::string emotion;
	float emotionStrength;
	float reactionStrength;

	Emotion();
	Emotion(std::string name);
	Emotion(std::string name, float emotionStrength);
	Emotion(std::string name, float emotionStrength, float reactionStrength);
};