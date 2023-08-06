#pragma once
#include "VAO.h"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>

class CubeMap {
public:
		/**
		*	@brief default constructor
		*	@details sets defualt values for a generic cubemap setup.
		*	shader files for the cubemap are assumed to be in:
		*		-"shaders/vert_cube_map.vert"
		*		-"shaders/frag_cube_map.frag"
		*/
	CubeMap();
	    /**
        *	@brief constructor with texture data.
        *   @param nTextures vector containing all 6 filepaths to cubemap textures.
        */
	CubeMap(std::vector<std::string>& nTextures);
		/**
		*	@brief draws cubemap texture as skybox.
		*	@details draws a skybox using the cubemap texture from the
		*	cameras POV. Skybox is rendered with maximum depth in the depth buffer
		*	so will always be draw behind other vertecies.
		*	@param camera Camera object to be drawn using
		*/
	void Render(Camera* camera);
		/**
		*	@brief Sets all 6 textures for the cubemap
		*	@warning Function expects textures in the following order:
		*	right, left, top, bottom, front, back.
		*	@param nTextures vector containing all 6 filepaths to cubemap textures.
		*/
	void SetTextures(std::vector<std::string>& nTextures);
		///Shader used to render cubemap
	Shader shader;

	std::string name = "";
private:
		///Cubemap VAO
	VAO vao;
		///ID for cubemap texure
	unsigned int ID;
		///Texture details
	int width, height, numColorChannels;

};