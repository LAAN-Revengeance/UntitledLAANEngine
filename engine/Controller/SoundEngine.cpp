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