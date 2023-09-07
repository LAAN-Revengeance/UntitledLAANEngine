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
	engine->play2D(audio[audioName].c_str(), true);
}

void SoundEngine::addSound(std::string audioName, std::string audioFilePath)
{
	audio[audioName] = audioFilePath;
}

void SoundEngine::playSoundAtPosition(std::string audioName, glm::vec3 pos)
{
	engine->play3D(audio[audioName].c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, true);
}

void SoundEngine::playLoopAtPosition(std::string audioName, glm::vec3 pos)
{
	engine->play3D(audio[audioName].c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), true, false, true);
}

void SoundEngine::setUserPosition(glm::vec3 pos)
{
	engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(0,0,0));
}