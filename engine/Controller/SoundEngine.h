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

	void playSoundFromFile(std::string audioFilePath);

	void playSound(std::string audioName);

	void playLoop(std::string audioName);

	void addSound(std::string audioName, std::string audioFilePath);

	void playSoundAtPosition(std::string audioName, glm::vec3 pos);

	void playLoopAtPosition(std::string audioName, glm::vec3 pos);

	void setUserPosition(glm::vec3 pos);
private:

	irrklang::ISoundEngine* engine;
	std::map<std::string, std::string> audio;
};

