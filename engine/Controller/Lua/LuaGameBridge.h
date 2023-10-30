#pragma once
#include "LuaFunction.h"
#include <GaemEngine.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <SoundEngine.h>

/**
*	@Class LuaGameBridge
*	@brief Exposes the Gaem Engine to a lua state
* 
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 21/09/2023
*/
class LuaGameBridge
{
public:
	static void ExposeEngine(LuaManager* luaManager);
};
