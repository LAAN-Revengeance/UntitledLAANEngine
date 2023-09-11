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

	void PlaySoundFromFile(std::string audioFilePath);

	void PlaySound(std::string audioName);

	void PlayLoop(std::string audioName);

	void AddSound(std::string audioName, std::string audioFilePath);

	void PlaySoundAtPosition(std::string audioName, glm::vec3 pos);

	irrklang::ISound* PlayDynamicSound(std::string audioName, glm::vec3 pos);

	void PlayLoopAtPosition(std::string audioName, glm::vec3 pos);

	void SetUserPosition(glm::vec3 pos);

	//void SetAudioPosition(irrklang::ISound* audio, glm::vec3 pos);

	std::vector<std::string> GetAudioNames();
private:

	irrklang::ISoundEngine* engine;
	std::vector<std::string> audioNames;
	std::map<std::string, std::string> audio;
	//std::map<std::string, irrklang::ISound*> dynamicAudio;
};

