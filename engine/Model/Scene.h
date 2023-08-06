#pragma once
#include "GameObject.h"
#include "Terrain.h"
#include "NPC.h"
#include "Lighting.h"
#include <Physics/Physics.h>
#include <map>
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
	GameObject& GetObject(std::string objName);

		/**
		*	@brief Get an npc in the scene based on its name
		*	@param objName name of npc to be retrieved
		*	@return npc with name objName
		*/
	NPC& GetNPC(std::string objName);

		/**
		*	@brief Get an object in the scene based on its ID
		*	@param objID ID of object to be retrieved
		*	@return Game object pointer with name objID
		*/
	GameObject* GetObjectByID(int objID);

		///This scenes camera
	Camera camera;
		///This scene skybox
	CubeMap* skybox = nullptr;
		///This scenes lighting setup
	Lights lights;
		///Physics world associated with this senes game objects
	Physics physics;
		///All game objects in this scene
	std::map<std::string,GameObject*> gameObjects;
		///same as gameObjects but by key is ID.
	std::map<int, GameObject*> gameObjectsID;
private:
};