#pragma once
#include "GameObject.h"

/**
*	@Class Terrain
*	@brief GameObject subclass that stores terrain
*
*	@author Andres Comeros-Ochtman
*	@version 2.0
*	@date 15/04/2023
*/
class Terrain : public GameObject
{
public:
		/**
		*	@brief default constructor
		*/
	Terrain();

		/**
		*	@brief constructor with filepath to heightmap
		*	@param nHeightMap filePath to heightmap
		*	@param xScale how scaled terrain is on x axis
		*	@param yScale how scaled terrain is on y axis
		*	@param zScale how scaled terrain is on z axis
		*/
	Terrain(const std::string& nHeightMap,float xScale, float yScale, float zScale, float texScale);

		/**
		*	@brief constructor with texture heightmap
		*	@param nHeightMap heightmap texture
		*	@param xScale how scaled terrain is on x axis
		*	@param yScale how scaled terrain is on y axis
		*	@param zScale how scaled terrain is on z axis
		*/
	Terrain(Texture* nHeightMap, float xScale, float yScale, float zScale, float texScale);

		/**
		*	@brief constructor with no height values, creates flat plane
		*	@param xScale how scaled terrain is on x axis
		*	@param zScale how scaled terrain is on z axis
		*/
	Terrain(int size, float xScale, float zScale, float texScale);

		/**
		*	@brief constructor with height values
		*	@param nHeights array of height values
		*/
	Terrain(std::vector<float> nHeights);

		/**
		*	@brief destructor
		*/
	~Terrain();

		/**
		*	@brief Sets terrain uniforms
		*	@return void
		*/
	virtual void SetUniforms();

		/**
		*	@brief Returns the height of the terrain at a specified x,z coord
		*	@param x x positon being sampled
		*	@param z z positon being sampled
		*	@return the y position of the terrain at x,z location
		*/
	float GetHeight(float x, float z);

		/**
		*	@brief Returns the normal of the terrain at a specified x,z coord
		*	@param x x positon being sampled
		*	@param z z positon being sampled
		*	@return the normal of the triangle of the terrain at x,z location
		*/
	glm::vec3 GetNormal(float x, float z);
		
		/**
		*	@brief Returns terrain size along the x and z axis
		*	@return terrain size along the x and z axis
		*/
	int GetSize();

		/**
		*	@brief return the highest point value on the terrain
		*	@return max height
		*/
	float GetMaxHeight();

		/**
		*	@brief return the lowest point value on the terrain
		*	@return min height
		*/
	float GetMinHeight();

		/**
		*	@brief return height array of the terrain
		*	@return pointer to the height array of the terrain
		*/
	std::vector<float>* GetHeightArray();

		/**
		*	@brief Sets textures of the terrain layers
		*	@param textures array of textures, one for each layer
		*	@param detailMap texture to overlay the other textures to give them more detail
		*	@return void
		*/
	void SetTextures(std::vector<Texture*> textures, Texture* detailMap);
		/**
		*	@brief Sets textures of the terrain layers
		*	@param textures array of textures, one for each layer
		*	@return void
		*/
	void SetMaterailTextures(std::vector<Texture*> textures);
		/**
		*	@brief Set the height of each texutre layer of the terrain
		*	@param heights height of each texture layer
		*	@return void
		*/
	void SetTextureHeights(std::vector<float> heights);

		/**
		*	@brief Sets the scale of the texures on the terrain
		*	@param nScale new texure scale
		*	@return void
		*/
	void SetTextureScale(float nScale);

		/**
		*	@brief loads the height texture map
		*	@param texture 
		*	@return void
		*/
	void SetHeightTexture(Texture* nHeightTex);

		/**
		*	@brief Loads a heightmap and generates model data based on it
		*	@param fileName file path to heightmap
		*	@return void
		*/
	void LoadHeightMap(const std::string& fileName);

		/**
		*	@brief Loads a heightmap and generates model data based on it
		*	@param nHeightMap heightmap texture
		*	@return void
		*/
	void LoadHeightMap(Texture* nHeightMap);

		/**
		*	@brief Loads a height array and generates model data based on it
		*	@param nHeightMap heights array
		*	@return void
		*/
	void LoadHeightMap(std::vector<float> nHeights);

	/**
	*	@brief returns the height texture of the terrain
	*	@return Texture
	*/
	Texture* GetHeightTexture();

		///scale along x axis
	float scaleX = 1.0f;
		///scale along y axis
	float scaleY = 1.0f;
		///scale along z axis
	float scaleZ = 1.0f;

	///creates height values from array from heightTexture
	void CreateHeightArray();


private:

	///generates the vertex buffer from height array
	void GenerateModel();

	///how often terrain textures repeat
	float textureScale = 10.0f;

		///square size of terrain
	int terrainSize;

		///Max amount of texure inits the terrain can use
	const int MAX_TERRAIN_TEXTURES = 11;

		///max height pre scaling the terrain can have
	float maxHeight = -100;
		///min height pre scaling the terrain can have
	float minHeight = 100;

		///Y values of the vertecies
	std::vector<float> *heightArray;
		///height map texture
	Texture* heightTexture = nullptr;

		///Texture unit numbers this terrain uses
	std::vector<int> tUnits;
		///Amount of texture units being used
	int tCount = 0;
};
