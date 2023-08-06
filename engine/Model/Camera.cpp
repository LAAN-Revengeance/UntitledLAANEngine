#include"Camera.h"
#include <iostream>

Camera::Camera()
{	
	position = glm::vec3(0.0f,0.0f,0.0f);
	rotation = glm::vec3(0.0f,0.0f,0.0f);

	front = glm::vec3(0.0f, 0.0f, 1.0f);
	right = glm::vec3(0.0f, 0.0f, -1.0f);
	up	 = glm::vec3(0.0f, 1.0f, 0.0f);

	FOV = 45.0f;
	aspectRatio =  16.0f / 9.0f;
	nearPlane = 0.1f;
	farPlane = 5000.0f;

	Yaw = 90.0f;
	Pitch = 0.0f;
	UpdateCameraVectors();
}

Camera::Camera(float nFOV, float nAspect, float nNear, float nFar)
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	front = glm::vec3(0.0f, 0.0f, 3.0f);
	right = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);

	FOV = nFOV;
	aspectRatio = nAspect;
	farPlane = nFar;
	nearPlane = nNear;

	Yaw = 90.0f;
	Pitch = 0.0f;
	UpdateCameraVectors();
}

void Camera::LookAt(glm::vec3 lookPos) {
	 front = glm::normalize(lookPos - position);
}

glm::mat4 Camera::GetView() 
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjection() {

	return glm::perspective(glm::radians(FOV), aspectRatio, nearPlane, farPlane);
}

void Camera::UpdateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 Front;
	Front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front.y = sin(glm::radians(Pitch));
	Front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front = glm::normalize(Front);
	// also re-calculate the Right and Up vector
	right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	up = glm::normalize(glm::cross(right, front));
}

void Camera::CreateViewFrustum()
{
	const float halfVSide = farPlane * tanf(90 * 0.5f);
	const float halfHSide = halfVSide * aspectRatio;
	const glm::vec3 frontMultFar = farPlane * front;

	frustum.near = { position + nearPlane * front, front };
	frustum.far = { position + frontMultFar, -front};
	frustum.right = { position,
							glm::cross(frontMultFar - right * halfHSide, up) };
	frustum.left = { position,
							glm::cross(up,frontMultFar + right * halfHSide) };
	frustum.top = { position,
							glm::cross(right, frontMultFar - up * halfVSide) };
	frustum.bottom = { position,
							glm::cross(frontMultFar + up * halfVSide,right) };
}
