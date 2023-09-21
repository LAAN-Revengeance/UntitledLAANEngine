#pragma once
#include "LuaManager.h"

class LuaGameBridge
{
public:
	static void ExposeEngine(LuaManager* luaState);
};
