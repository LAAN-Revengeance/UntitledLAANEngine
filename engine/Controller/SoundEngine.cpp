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

void SoundEngine::PlayStaticSound(std::string audioName)
{
	//check if ISound is already set
	if (audio.find(audioName) != audio.end())
		audio[audioName].sound = engine->play2D(audio[audioName].filepath.c_str());
}

void SoundEngine::PlayDynamicSound(std::string audioName, glm::vec3 pos)
{
	if (audio.find(audioName) != audio.end())
		audio[audioName].sound = engine->play3D(audio[audioName].filepath.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, true);

	//if (audio[audioName].sound)
	//	engine->play3D(audio[audioName].filepath.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, true);
	//else
	//	audio[audioName].sound = engine->play3D(audio[audioName].filepath.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, true);
}

void SoundEngine::PlayLoop(std::string audioName)
{
	//check if ISound is already set
	if (audio[audioName].sound)
		engine->play2D(audio[audioName].filepath.c_str(), true);
	else
		audio[audioName].sound = engine->play2D(audio[audioName].filepath.c_str(), true);
}

void SoundEngine::AddSound(std::string audioName, std::string audioFilePath)
{
	audio[audioName].filepath = audioFilePath;
	//audioNames.push_back(audioName);
}

void SoundEngine::RemoveSound(std::string audioName)
{
	//erase sound 
	audio[audioName].sound->drop();

	//del from map
	audio.erase(audioName);

	//del from vector
	//std::vector<std::string>::iterator i = std::find(audioNames.begin(), audioNames.end(), audioName);
	//if (i != audioNames.end()) 
	//	audioNames.erase(i);
}



void SoundEngine::PlayLoopAtPosition(std::string audioName, glm::vec3 pos)
{
	//check if ISound is already set
	if (audio[audioName].sound)
		engine->play3D(audio[audioName].filepath.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), true, false, true);
	else 
		audio[audioName].sound = engine->play3D(audio[audioName].filepath.c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, true);
}

void SoundEngine::SetUserPosition(glm::vec3 pos)
{
	engine->setListenerPosition(irrklang::vec3df(pos.x, pos.y, pos.z), irrklang::vec3df(0,0,0));
}

void SoundEngine::UpdateUserPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 upDir)
{
	irrklang::vec3df position(pos.x, pos.y, pos.z);						// position of the listener
	irrklang::vec3df lookDirection(lookDir.x, lookDir.y, lookDir.z);	// the direction the listener looks into
	irrklang::vec3df velPerSecond(velocity.x, velocity.y, velocity.z);  // only relevant for doppler effects
	irrklang::vec3df upVector(upDir.x, upDir.y, upDir.z);				// where 'up' is in your 3D scene

	engine->setListenerPosition(position, lookDirection, velPerSecond, upVector);
}

void SoundEngine::SetSoundPause(std::string audioName, bool pause)
{
	if (audio[audioName].sound)
		audio[audioName].sound->setIsPaused(pause);
}

bool SoundEngine::GetSoundPause(std::string audioName)
{
	return audio[audioName].sound->getIsPaused();
}

void SoundEngine::SetSoundLoop(std::string audioName, bool loop)
{
	if (audio[audioName].sound)
		audio[audioName].sound->setIsLooped(loop);
}

bool SoundEngine::GetSoundLoop(std::string audioName)
{
	return audio[audioName].sound->isLooped();
}

void SoundEngine::SetSoundDistance(std::string audioName, float minDist)
{
	if (audio[audioName].sound)
	{
		audio[audioName].sound->setMinDistance(minDist);
	}
}

void SoundEngine::SetVolume(std::string audioName, float volume)
{
	if (audio[audioName].sound)
		audio[audioName].sound->setVolume(volume);
}

float SoundEngine::GetVolume(std::string audioName)
{
	return audio[audioName].sound->getVolume();;
}

void SoundEngine::SetAudioPosition(std::string audioName, glm::vec3 pos)
{
	irrklang::vec3df position(pos.x, pos.y, pos.z);

	if (audio[audioName].sound)
		audio[audioName].sound->setPosition(position);
}


std::map<std::string, audioData> SoundEngine::GetAudioNames()
{
	return audio;
}

void SoundEngine::UpdateDynamicAudio()
{

}


