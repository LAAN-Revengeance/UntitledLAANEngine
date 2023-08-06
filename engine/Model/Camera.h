#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <Frustum.h>

#define PI 3.14159265

/**
*	@Class Camera
*	@brief Stores camera properties required top render graphics to the
*	screen. provides functionality to use camera properties to return view and
*	projection matricies
*
*	@author Andres Comeros-Ochtman
*	@version 1.0
*	@date 15/04/2023
*/
class Camera {
public:
		/**
		*	@brief Default constructor. Sets generic camera values.
		*/
	Camera();

		/**
		*	@brief constructor with inital values.
		*	@param nAspect inital aspect ratio
		*	@param nNear inital near clipping plane
		*	@param nFar inital far clipping plane
		*/
	Camera(float nFOV,float nAspect, float nNear, float nFar);

		/**
		*	@brief Rotate camera to look at a location
		*	@param lookPos location to look at
		*	@return void
		*/
	void LookAt(glm::vec3 lookPos);

		/**
		*	@brief Get the view matrix
		*	@return view matrix constructed from camera data
		*/
	glm::mat4 GetView();

		/**
		*	@brief Get the projection matrix
		*	@return projection matrix constructed from camera data
		*/
	glm::mat4 GetProjection();

		/**
		*	@brief Recalculate front and right vectors
		*	@return void
		*/
	void UpdateCameraVectors();

		/**
		*	@brief calculates the currrent view fustum volume and stores it in frustum
		*	@return void
		*/
	void CreateViewFrustum();

		///Cameras rotation in degrees
	glm::vec3 rotation;
		///Cameras positon in meters
	glm::vec3 position;
		///Front vector
	glm::vec3 front;
		///Up vector
	glm::vec3 up;
		///Right Vector
	glm::vec3 right;

	// euler Angles
	float Yaw;
	float Pitch;
		
		///Field of view
	float FOV;
		///Aspect ratio
	float aspectRatio;
		///Near clipping plane
	float nearPlane;
		///Far clipping plane
	float farPlane;

	Frustum frustum;

private:

};