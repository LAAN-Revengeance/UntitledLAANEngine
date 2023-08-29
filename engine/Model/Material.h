#pragma once
#include <vector>
#include <Graphics/Graphics.h>

/**
*	@Struct Material
*	@brief stores material data for rendering an object
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
struct Material {
	std::vector<Texture*> diffuseTexture;
	std::vector<Texture*> specularMap;
	std::vector<Texture*> emissionMap;
	std::vector<Texture*> normalMap;

	float shine = 1;
};