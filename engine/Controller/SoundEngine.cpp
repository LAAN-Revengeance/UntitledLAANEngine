#include "SoundEngine.h"
#include <iostream>


SoundEngine& SoundEngine::Get() {

	static SoundEngine e_instance;
	return e_instance;
}

SoundEngine::SoundEngine()
{
	_Engine = irrklang::createIrrKlangDevice();
}

void SoundEngine::addSound(std::string name, std::string filePath)
{
	_Audio.insert({ name, filePath });
}

void SoundEngine::playSound(std::string name, glm::vec3 pos)
{
	_Engine->play3D(_Audio.at(name).c_str(), irrklang::vec3df(pos.x, pos.y, pos.z), false, false, false);
}

void SoundEngine::addMusic(std::string filePath)
{
	_Music = _Engine->play3D(filePath.c_str(), { 0, 0, 0 }, true, false, true);
	_Music->setVolume(0.2f);
}

void SoundEngine::setVolume(float volume)
{
	_Engine->setSoundVolume(volume);
}

void SoundEngine::setMusicVolume(float volume)
{
	_Music->setVolume(volume);
}

void SoundEngine::setListenerPos(glm::vec3 pos)
{
	_Engine->setListenerPosition(irrklang::vec3df( pos.x, pos.y, pos.z ), { 0, 0, 1 });
}

void SoundEngine::setMusicPos(glm::vec3 pos)
{
	if(_Music)
	_Music->setPosition(irrklang::vec3df(pos.x, pos.y, pos.z));
}

void SoundEngine::toggleMusic(bool state)
{
	if (_Music)
	_Music->setIsPaused(state);
}

void SoundEngine::drop()
{
	_Music->drop();
	_Engine->drop();
}

void SoundEngine::playSoundLoop(std::string name, glm::vec3 pos, float vol)
{
	if (loopedAudio.find(name) == loopedAudio.end()) {
		irrklang::ISound* nLoop = _Engine->play3D(_Audio[name].c_str(), { pos.x, pos.y, pos.z }, true, false, true);
		loopedAudio.insert({ name,nLoop });
	}
	else {
		if (loopedAudio[name]->getIsPaused()) {
			loopedAudio[name]->setIsPaused(false);
		}

	}
	loopedAudio[name]->setVolume(vol);
	loopedAudio[name]->setPosition({ pos.x,pos.y,pos.z });
}

void SoundEngine::stopSoundLoop(std::string name)
{
	if (loopedAudio.find(name) != loopedAudio.end()) {
		loopedAudio.at(name)->setIsPaused(true);
	}
}
