#include <AI/Emotion/Emotion.h>

Emotion::Emotion()
{
	this->emotionStrength = 0;
	this->reactionStrength = 0;
}

Emotion::Emotion(std::string name)
{
	this->emotion = name;
	this->emotionStrength = 0;
	this->reactionStrength = 0;
}

Emotion::Emotion(std::string name, float emotionStrength)
{
	this->emotion = name;
	this->emotionStrength = emotionStrength;
	this->reactionStrength = 0;
}

Emotion::Emotion(std::string name, float emotionStrength, float reactionStrength)
{
	this->emotion = name;
	this->emotionStrength = emotionStrength;
	this->reactionStrength = reactionStrength;
}