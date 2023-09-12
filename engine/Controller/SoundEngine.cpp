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

void SoundEngine::PlaySoundFromFile(std::string audioFilePath)
{
	engine->play2D(audioFilePath.c_str());
}

void SoundEngine::PlaySound(std::string audioName)
{
	engine->play2D(audio[audioName].c_str());
}

void SoundEngine::PlayLoop(std::string audioName)
{
	engine->play2D(audio[audioName].c_str(), true);
}

void SoundEngine::AddSound(std::string audioName, std::string audioFilePath)
{
	audio[audioName] = audioFilePath;
	audioNames.push_back(audioName);
}

void SoundEngine::PlaySoundAtPosition(std::string audioName, glm::vec3 pos)
{
	engine->play3D(audio[audioName].c_str(), vec3D(pos.x, pos.y, pos.z), false, false, true);
}

void SoundEngine::PlayDynamicSound(std::string audioName, glm::vec3 pos)
{
	engine->play3D(audio[audioName].c_str(), vec3D(pos.x, pos.y, pos.z), false, false, true);
}

void SoundEngine::PlayLoopAtPosition(std::string audioName, glm::vec3 pos)
{
	engine->play3D(audio[audioName].c_str(), vec3D(pos.x, pos.y, pos.z), true, false, true);
}

void SoundEngine::SetUserPosition(glm::vec3 pos)
{
	engine->setListenerPosition(vec3D(pos.x, pos.y, pos.z), vec3D(0,0,0));
}


/*
* 
* WIP: Need to figure out a better method for dynamic audio for milestone 2
* 
void SoundEngine::SetAudioPosition(irrklang::ISound* audio, glm::vec3 pos)
{
	if (audio)
	{
		audio->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));
	}
}
*/

std::vector<std::string> SoundEngine::GetAudioNames()
{
	return audioNames;
}