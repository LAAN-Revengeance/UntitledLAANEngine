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
private:

	irrklang::ISoundEngine* _Engine;
};

