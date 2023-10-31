#pragma once
#include "SceneLoader.h"
#include <GaemEngine.h>

/**
*	@Struct Project
*	@brief Used store project data
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 12/09/2023
*/
struct Project {

	///Title of window when project is running
	std::string windowName = "";

	///Filepath to main lua file
	std::string luaPath = "";

	///Scene the project is centered around
	Scene* scene;
};

/**
*	@Class SceneLoader
*	@brief Used to serialize and de-serialize a project in JSON format
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 12/09/2023
*/
class ProjectLoader
{
public:

	/**
	*	@brief Serializes a project in JSON format and stores externally
	*	@param scene scene to be serialized
	*	@param luaFile path to file containing main.lua
	*	@param luaFile name of this project
	*	@param outName file path to save scene JSON file
	*	@return void
	*/
	static void SaveProject(Scene* scene, const std::string luaFile, const std::string windowName, const std::string outName);

	/**
	*	@brief loads a project from a JSON file
	*	@param inName filePath of project file
	*	@return Project
	*/
	static Project LoadProject(GameEngine* engine, const char* inName);

private:
	ProjectLoader();
	~ProjectLoader();
};


