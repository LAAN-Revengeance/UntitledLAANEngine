#pragma once
#include <iostream>

class Emotion
{
public:
	std::string emotion = "";
	float emotionStrength = 0;
	float reactionStrength = 0;

	Emotion();
	Emotion(std::string name);
	Emotion(std::string name, float emotionStrength);
	Emotion(std::string name, float emotionStrength, float reactionStrength);
};