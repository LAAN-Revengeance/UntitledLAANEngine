#pragma once
#include "LuaManager.h"
#include <Scene.h>
#include <GameObject.h>
#include <Physics/PhysicsManager.h>
#include <ResourceManager.h>
#include <InputManager.h>
#include <GUIRenderer.h>
#include <Renderer.h>
#include <SoundEngine.h>
#include <Serialization/SceneLoader.h>

class LuaGameBridge
{
public:
	static void ExposeEngine(LuaManager* luaState);
};
