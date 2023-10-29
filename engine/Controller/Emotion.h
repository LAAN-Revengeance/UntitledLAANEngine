#pragma once
#include <iostream>

class Emotion
{
public:
	Emotion();
	~Emotion();
	Emotion(std::string name);
	Emotion(std::string name, float emotionStrength);
	Emotion(std::string name, float emotionStrength, float reactionStrength);

	void SetEmotion(std::string emotionName);
	void SetEmotionStrength(float emotionStrength);
	void SetReactionStrength(float reactionStrength);

	std::string GetEmotion();
	float GetEmotionStrength();
	float GetReactionStrength();
private:
	std::string emotion;
	float emotionStrength;
	float reactionStrength;
};