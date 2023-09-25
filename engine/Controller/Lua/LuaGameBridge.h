#pragma once
#include "LuaManager.h"
#include <GaemEngine.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <SoundEngine.h>


class LuaGameBridge
{
public:
	static void ExposeEngine(GameEngine*);
};
