#pragma once
#include <irrKlang/irrKlang.h>
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#pragma comment(lib, "irrKlang.lib")

/**
*	@class SoundEngine
*	@brief contains basic function play sounds within a 3D space
*	@version 1
*	@author Dylan Burgess
*	@date 07/06/2023
*/

class SoundEngine
{
public:
	/**
	*	@brief default constructor
	*/
	SoundEngine();

	/**
	*	@brief gets the sound engine singleton
	*	@return SoundEngine
	*/
	static SoundEngine& Get();
	/**
	*	@brief adds sounds to the sound map
	*   @param name - the key for the map
	*   @param filePath - location of the sound
	*	@return void
	*/
	void addSound(std::string name, std::string filePath);

	/**
	*	@brief plays a sounds from the sound map
	*   @param name - the sound to play
	*   @param pos - position of the sound to play
	*	@return void
	*/
	void playSound(std::string name, glm::vec3 pos);

	/**
	*	@brief adds music to the sound engine
	*	@param filePath - location of the music
	*	@return void
	*/
	void addMusic(std::string filePath);
	/**
	*	@brief set the volume of the sound engine
	*	@param volume the perctane of volume 0-1
	*	@return void
	*/
	void setVolume(float volume);

	/**
	*	@brief set the volume of the music engine
	*	@param volume the perctane of volume 0-1
	*	@return void
	*/
	void setMusicVolume(float volume);

	/**
	*	@brief set the position of the sound listener
	*	@param pos - position of the listener
	*	@return void
	*/
	void setListenerPos(glm::vec3 pos);
	/**
	*	@brief set the position of the music player
	*	@param pos - position of the music player
	*	@return void
	*/
	void setMusicPos(glm::vec3 pos);

	/**
	*	@brief turns on or off the music
	*	@param state - true for on / false for off
	*	@return void
	*/
	void toggleMusic(bool state);
	/**
	*	@brief memory cleanup
	*	@return void
	*/
	void drop();

	/**
	*	@brief plays a looped sound
	*	@param name - the sound to play
	*	@param pos - position of the sound to play
	* 	@param volume the perctane of volume 0-1
	*	@return void
	*/
	void playSoundLoop(std::string name, glm::vec3 pos, float vol);


	/**
	*	@brief stops a looped sound
	*	@param name - the sound to play
	*	@return void
	*/
	void stopSoundLoop(std::string name);

private:

	irrklang::ISoundEngine* _Engine;
	std::map<std::string, std::string> _Audio;
	irrklang::ISound* _Music;

	std::map<std::string, irrklang::ISound*> loopedAudio;
};

