#include"camera.h"
#include <iostream>

Camera::Camera(glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, glm::vec3 up /*= glm::vec3(0.0f, 1.0f, 0.0f)*/, float yaw /*= YAW*/, float pitch /*= PITCH*/)
{
	Position = position;
	Up = up;
	Front = glm::vec3(0, 0, -1);
	Right = glm::vec3(1, 0, 0);

	Yaw = -90.0f;
	Pitch = 0.0f;
	WorldUp = glm::vec3(0, 1,0);
}

void Camera::ProcessMouseMovement(double xoffset, double yoffset)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;


	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;


	updateCameraVectors();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime * 3;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity ;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == UP)
		Position -= Up * velocity;
	if (direction == DOWN)
		Position += Up * velocity;
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

