#pragma once
#include <ResourceManager.h>
#include <Physics/PhysicsManager.h>
#include <Scene.h>
#include <Terrain.h>
#include <json/json.h>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
/**
*	@Class SceneLoader
*	@brief Used to serialize and de-serialize a scene in JSON format
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 20/05/2023
*/
class SceneLoader
{
public:

		/**
		*	@brief Default constructor
		*/
	SceneLoader();

		/**
		*	@brief deconstrucor
		*/
	~SceneLoader();

		/**
		*	@brief Serializes a scene in JSON format and stores externally
		*	@param scene scene to be serialized
		*	@param outName file path to save scene JSON file 
		*	@return void
		*/
	static void SaveScene(Scene* scene, const std::string outName);

		/**
		*	@brief loads a scene from a JSON file
		*	@param inName filePath of save file
		*	@return Scene referne to newly created scene
		*/
	static Scene& LoadScene(const char* inName);

		/**
		*	@brief Returns names of all saves in a folder
		*	@param path path to folder containing saves
		*	@return std::vector<std::string> vector of save names
		*/
	static std::vector<std::string> GetSaves(const std::string path);

private:

		/**
		*	@brief Serializes a GameObject
		*	@param obj object to be serialized
		*	@return Json::Value obj as json object
		*/
	static Json::Value ObjectToJson(GameObject* obj);

		/**
		*	@brief Serializes lighting
		*	@param lights Lights object to be serialized
		*	@return Json::Value lights as json object
		*/
	static Json::Value LightsToJson(Lights* lights);

};
