#pragma once
#include <glm/glm.hpp>
#include <vector>

	/**
	* @struct PointLight
	* @brief Point light data
	* @details Point lights effect attenuates with distance
	* uniformly in all directions.
	*/
struct PointLight {
	glm::vec3 position;
	glm::vec3 diffuse;
	glm::vec3 specular;

	//attenuation values
	float constant;
	float linear;
	float quadratic;

	PointLight() : position(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)),
		constant(0.0f), linear(0.0f), quadratic(0.0f) {}

	PointLight(const glm::vec3& position, const glm::vec3& diffuse, const glm::vec3& specular,
		float constant, float linear, float quadratic)
		: position(position), diffuse(diffuse), specular(specular),
		constant(constant), linear(linear), quadratic(quadratic) {}
};

		/**
		* @struct SpotLight
		* @brief SpotLight light data
		* @details Spot lights point in a direction across
		* maxAngle degrees and attenuate in affect in that direction.
		*/
struct SpotLight {
	glm::vec3 position;
	glm::vec3 direction;
	float maxAngle;
	float featherAngle;

	glm::vec3 diffuse;
	glm::vec3 specular;

	//attenuation values
	float constant;
	float linear;
	float quadratic;

	SpotLight() : position(glm::vec3(0.0f)), direction(glm::vec3(0.0f)), maxAngle(0.0f), featherAngle(0.0f),
		diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)), constant(0.0f), linear(0.0f), quadratic(0.0f) {}

	SpotLight(const glm::vec3& position, const glm::vec3& direction, float maxAngle, float featherAngle,
		const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic)
		: position(position), direction(direction), maxAngle(maxAngle), featherAngle(featherAngle),
		diffuse(diffuse), specular(specular), constant(constant), linear(linear), quadratic(quadratic) {}
};
	/**
	* @struct DirectionLight
	* @brief light pointing on all surfaces uniformly from a specified direction
	* @details Simulates light from infinite distance away pointing in a specified vector.
	*/
struct DirectionLight {
	glm::vec3 direction;
	glm::vec3 diffuse;
	glm::vec3 specular;

	DirectionLight() : direction(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)) {}

	DirectionLight(const glm::vec3& direction, const glm::vec3& diffuse, const glm::vec3& specular)
		: direction(direction), diffuse(diffuse), specular(specular) {}
};

	/**
	*	@struct Lights
	*	@brief Collection of lights for a scene
	*	@details stores all lights a scene may use
	*/
struct Lights {
	glm::vec3 ambient = {1,1,1};
	std::vector<PointLight> point;
	std::vector<SpotLight> spot;
	std::vector<DirectionLight> direction;

	void SetAmbient(float r, float g, float b){
		ambient = { r,g,b };
	}

	void AddDirectionLight(const glm::vec3& ndirection, const glm::vec3& ndiffuse, const glm::vec3& nspecular) {
		DirectionLight ndir(ndirection, ndiffuse, nspecular);
		direction.push_back(ndir);
	}

	void AddSpotLight(const glm::vec3& position, const glm::vec3& ndirection, float maxAngle, float featherAngle,
		const glm::vec3& diffuse, const glm::vec3& specular, float constant, float linear, float quadratic)
	{
		SpotLight nspt(position,ndirection,maxAngle,featherAngle,diffuse,specular,constant,linear,quadratic);
		spot.push_back(nspt);
	}

	void AddPointLight(const glm::vec3& position, const glm::vec3& diffuse, const glm::vec3& specular,
		float constant, float linear, float quadratic) 
	{
		PointLight npnt(position,diffuse,specular,constant,linear,quadratic);
		point.push_back(npnt);
	}
};

