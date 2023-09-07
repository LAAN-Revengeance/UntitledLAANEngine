#pragma once
#include <irrKlang/irrKlang.h>
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#pragma comment(lib, "irrKlang.lib")

class SoundEngine
{
public:
	SoundEngine();

	static SoundEngine& Get();

	void playSound(std::string audioFilePath);

	void playLoop(std::string audioFilePath);
private:

	irrklang::ISoundEngine* _Engine;
};

