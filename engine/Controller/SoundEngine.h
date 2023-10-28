#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <irrKlang/irrKlang.h>
#pragma comment(lib, "irrKlang.lib")

#define AudioEngine irrklang::ISoundEngine*
#define vec3D irrklang::vec3df

using namespace irrklang;

struct audioData
{
	std::string filepath;
	ISound* sound;
};

class SoundEngine
{
public:
	SoundEngine();

	static SoundEngine& Get();

	void PlaySoundFromFile(std::string audioFilePath);

	void PlaySound(std::string audioName);
	void PlayDynamicSound(std::string audioName, glm::vec3 pos);

	void PlayLoop(std::string audioName);

	void AddSound(std::string audioName, std::string audioFilePath);
	void RemoveSound(std::string audioName);

	void PlayLoopAtPosition(std::string audioName, glm::vec3 pos);

	void SetUserPosition(glm::vec3 pos);
	void UpdateUserPosition(glm::vec3 pos, glm::vec3 lookDir, glm::vec3 velocity, glm::vec3 upDir);

	void SetSoundPause(std::string audioName, bool pause);
	bool GetSoundPause(std::string audioName);

	void SetSoundLoop(std::string audioName, bool loop);
	bool GetSoundLoop(std::string audioName);

	void SetSoundDistance(std::string audioName, float minDist);

	void SetVolume(std::string audioName, float volume);
	float GetVolume(std::string audioName);

	void SetAudioPosition(std::string audioName, glm::vec3 pos);

	std::vector<std::string> GetAudioNames();

	void UpdateDynamicAudio();
private:

	AudioEngine engine;
	std::vector<std::string> audioNames;
	std::map<std::string, audioData> audio;
	//std::map<std::string, irrklang::ISound*> dynamicAudio;
};

