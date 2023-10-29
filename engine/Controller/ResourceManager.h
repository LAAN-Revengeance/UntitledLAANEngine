#pragma once
#include <map>
#include <Gameobject.h>
#include <Mesh.h>
#include <MD2/MD2Reader.h>
#include <Terrain.h>
#include <GameObjects/NPC_GameObject.h>
#include <array>

/**
*	@Class ResourceManager
*	@brief  Utilises the Abstract factory method for game object creation.
*	A singleton class used to load textures,shaders and 3D models.
*	Resource manager allows for resources to be shared between objects as
*	well as providing an easy way to access resources.
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class ResourceManager
{
public:
		/**
		*	@brief Get the singleton instance of ResourceManager
		*	@return The singleton instance of ResourceManager
		*/
	static ResourceManager& Get();


	Material CreateMaterial(std::string diff, std::string spec, std::string emiss);
		/**
		*	@brief Abstract factory method for creating game objects
		*	@param objectName name of created game object
		*	@param modelName name key of model data
		*	@param shaderName name key for shader data, use "" to use default shader
		*	@return a game object created using the selected parameters
		*/
	GameObject& CreateGameObject(std::string objectName, std::string modelName, std::string shaderName, Material material = Material());

		/**
		*	@brief Abstract factory method for creating NPC game objects
		*	@param objectName name of created game object
		*	@param modelName name key of model data
		*	@param shaderName name key for shader data, use "" to use default shader
		*	@return a npc game object created using the selected parameters
		*/
	NPC_GameObject& CreateNPC(std::string objectName, std::string modelName, std::string shaderName, Material material = Material());

		/**
		*	@brief Abstract factory method for creating terrain game objects
		*	@param terrainName name of created game object
		*	@param heightMap name key of texture used as a heightmap
		*	@param layerTextures array of names used for each layer in terrain texture
		*	@param detailName name key of detail map of this terrain
		*	@param emissiveName name key of emmissive texture of this terrain
		*	@param pecularName name key of specular texture of this terrain
		*	@param texScale texture scale of terrain
		*	@param scaleX x axis scaling
		*	@param scaleY y axis scaling
		*	@param scaleZ z axis scaling
		*	@return a game object created using the selected parameters
		*/
	Terrain& CreateTerrain(std::string terrainName, std::string heightMapName, std::vector<std::string> layerTextures, std::string detailName, std::string specularName, std::string emissiveName, float texScale, float scaleX, float scaleY, float scaleZ);

		/**
		*	@brief loads a texture into storage
		*	@param	resName name key to assign to this resource
		*	@param	fileName file path of this resource
		*	@return void
		*/
	void LoadTexture(std::string resName, std::string fileName);
		/**
		*	@brief loads a md2 file into storage
		*	@param	resName name key to assign to this resource
		*	@param	fileName file path of this resource
		*	@return void
		*/
	void LoadAnimatedModel(std::string resName, std::string fileName);
		/**
		*	@brief loads a model into storage
		*	@param	resName name key to assign to this resource
		*	@param	fileName file path of this resource
		*	@return void 
		*/
	void LoadModel(std::string resName, std::string fileName);

		/**
		*	@brief loads a shader into storage
		*	@param	resName name key to assign to this resource
		*	@param	fileName file path of this resource
		*	@param	vertPath vertex shader filePath
		*	@param	fragpath fragment shader filePath
		*	@param	geomPath geometry shader filePath, enter "" to not use
		*	@return void
		*/
	void LoadShader	(std::string resName, std::string vertPath, std::string fragPath, std::string geomPath);

		/**
		*	@brief loads a cubemap into storage
		*	@param	resName name key to assign to this resource
		*	@param	fileName file path of this resource
		*	@param	right Right texture
		*	@param	left Left texture
		*	@param	top Top texture
		*	@param	bottom Bottom texture
		*	@param	front Front texture
		*	@param	back Back texture
		*	@return void
		*/
	void LoadCubemap(std::string resName, std::string right, std::string left, std::string top, std::string bottom, std::string front, std::string back);

		/**
		*	@brief Retrieve a texture from storage
		*	@param resName name key of resource
		*	@return a texture pointer
		*/
	Texture* GetTexture(std::string resName);

		/**
		*	@brief Retrieve a model from storage
		*	@param resName name key of resource
		*	@return model pointer
		*/
	DrawItem* GetModel(std::string resName);

		/**
		*	@brief Retrieve a shader from storage
		*	@param resName name key of resource
		*	@return shader pointer
		*/
	Shader* GetShader(std::string resName);

		/**
		*	@brief Retrieve a cubbemap from storage
		*	@param resName name key of resource
		*	@return cubemap pointer
		*/
	CubeMap* GetCubeMap(std::string resName);

		/**
		*	@brief Retrieve a gameobject from storage
		*	@param resName name key of resource
		*	@return gameobject pointer
		*/
	GameObject* GetGameObject(std::string resName);

		///Deletes a gameobject
	void DeleteGameObject(std::string goName);

		///Get itterator for texture resources
	auto TextureBegin() { return textures.begin(); }
		///Get itterator for texture resources
	auto TextureEnd() { return textures.end(); }

		///Get itterator for model resources					
	auto ModelBegin() { return models.begin(); }
		///Get itterator for model resources
	auto ModelEnd() { return models.end(); }

		///Get itterator for shader resources			
	auto ShaderBegin() { return shaders.begin(); }
		///Get itterator for shader resources
	auto ShaderEnd() { return shaders.end(); }

		///Get itterator for cubemap resources					
	auto CubeMapBegin() { return cubemaps.begin(); }
		///Get itterator for cubemap resources
	auto CubeMapEnd() { return cubemaps.end(); }
		///Get reference to draw item instead of pointer
	DrawItem& GetDrawItemReference(std::string resName);
		///Store a game object
	void StoreGameObject(GameObject* go);
		/**
		*	@brief data cleanUp
		*	@return Void
		*/
	void ClearGameObjects();
private:

		///ID of next game object created
	unsigned int IDIndex = 1;
		///Default constructor. Private because singleton
	ResourceManager();
		///Destructor
	~ResourceManager();
		///Copy constructor removed becuase singleton
	ResourceManager(const ResourceManager&) = delete;
		///Assignment operator removed becuase singleton
	ResourceManager& operator = (const ResourceManager&) = delete;
		
		///texture storage
	std::map<std::string, Texture*> textures;
	std::map<std::string, std::string> texturePaths;
		///model storage
	std::map<std::string, DrawItem*> models;
	std::map<std::string, std::string> modelPaths;
		///shader storage
	std::map<std::string, Shader*> shaders;
	std::map<std::string, std::array<std::string, 3>> shadersPaths;
		///cubemap storage
	std::map<std::string, CubeMap*> cubemaps;
	std::map<std::string, std::array<std::string,6>> cubemapPaths;
		///gameobject storage
	std::map<std::string, GameObject*> objects;

	friend class SceneEditor;
	friend class SceneLoader;
};

