#include "SoundEngine.h"
#include <iostream>

SoundEngine& SoundEngine::Get() {

	static SoundEngine e_instance;
	return e_instance;
}

SoundEngine::SoundEngine()
{
	engine = irrklang::createIrrKlangDevice();
}

void SoundEngine::playSoundFromFile(std::string audioFilePath)
{
	engine->play2D(audioFilePath.c_str());
}

void SoundEngine::playSound(std::string audioName)
{
	engine->play2D(audio[audioName].c_str());
}

void SoundEngine::playLoop(std::string audioName)
{
	engine->play2D(audio[audioName].c_str());
}

void SoundEngine::addSound(std::string audioName, std::string audioFilePath)
{
	audio[audioName] = audioFilePath;
}