#pragma once
#include "GameObject.h"
#include "Lighting.h"
#include <Physics/PhysicsManager.h>
#include <Lua/LuaFunction.h>
#include <Pathfinding/PathNodeManager.h>

/**
*	@Class Scene
*	@brief Stores all data required to render a scene. Also stores reference
*	to a physics world.
*
*	@author Andres Comeros-Ochtman, Dylan Burgess, Nathan Choo
*	@version 2.0
*	@date 15/04/2023
*/
class Scene
{
public:
		/**
		*	@brief default constructor
		*/
	Scene();

		/**
		*	@brief destructor
		*/
	~Scene();

		/**
		*	@brief assignment operator overload
		*/
	Scene& operator=(const Scene& other);

		/**
		*	@brief Add an object to the scene
		*	@param gameObject object to add to scene
		*	@return void
		*/
	void AddObject(GameObject& gameObject);

		/**
		*	@brief Add a light setup to the scene
		*	@param	nLights new scene lighting data
		*	@return void
		*/
	void AddLights(Lights& nLights);

		/**
		*	@brief Set the scenes skybox
		*	@param nSkybox skybox to set
		*	@return void
		*/
	void SetSkybox(CubeMap* nSkybox);

		/**
		*	@brief Get reference to the scenes camera
		*	@return the scenes camera
		*/
	Camera& GetCamera();

		/**
		*	@brief Returns refernece to this scenes lighting setup
		*	@return the scenes light setup
		*/
	Lights& GetLights();

		/**
		*	@brief Get an object in the scene based on its name
		*	@param objName name of object to be retrieved
		*	@return Game object with name objName
		*/
	GameObject& GetGameObject(std::string objName);

		///The luaState associated with this scene
	LuaManager luaState;
		///Update Lua function
	LuaFunction<void, double> UpdateFunction;
		///Init lua functoin
	LuaFunction<void> InitFunction;
		///Physics world
	PhysicsManager physicsWorld;
		///This scenes main camera
	Camera camera;
		///This scene skybox
	CubeMap* skybox = nullptr;
		///This scenes lighting setup
	Lights lights;
		///All game objects in this scene
	std::map<std::string,GameObject*> gameObjects;
		///AI navigation data
	GaemPathing::PathNodeManager pathManager;

private:
};