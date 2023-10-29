#include <Emotion.h>

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

void Emotion::SetEmotion(std::string emotionName)
{
	this->emotion = emotionName;
}

void Emotion::SetEmotionStrength(float emotionStrength)
{
	this->emotionStrength = emotionStrength;
}

void Emotion::SetReactionStrength(float reactionStrength)
{
	this->reactionStrength = reactionStrength;
}

std::string Emotion::GetEmotion()
{
	return this->emotion;
}

float Emotion::GetEmotionStrength()
{
	return this->emotionStrength;
}

float Emotion::GetReactionStrength()
{
	return this->reactionStrength;
}