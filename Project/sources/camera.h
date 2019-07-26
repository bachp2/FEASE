#pragma once

#include "globals.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

typedef glm::vec3 Vec3;
typedef glm::quat Quat;
typedef glm::mat4 Mat4;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

enum Camera_State {
	CAM_FIXED,
	CAM_FREE
};

// Default camera values
const float PITCH = 10.0f;
const float YAW = 20.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.005f;
const float ZOOM = 1.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class ArcBallCamera
{
public:
	// Camera Attributes
	Camera_State cameraState;
	float Pitch;
	float Yaw;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float ScrollSensitivity;
	float Zoom;
	
	ArcBallCamera(
		float pitch = PITCH, float yaw = YAW
		) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), ScrollSensitivity(SENSITIVITY*30), Zoom(ZOOM)
	{
		Pitch = pitch;
		Yaw = yaw;
		cameraState = CAM_FREE;
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{	
		auto model = GetModelMatrix();
		return LookAtMat*glm::inverse(model);
	}
	
	glm::mat4 getDefaultViewMatrix() {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, Vec3(1 / Zoom));
		return LookAtMat*model;
	}

	void rotateTo(float dstYaw, float dstPitch)
	{
		if (cameraState == CAM_FIXED) return;
		Pitch = dstPitch;
		Yaw = dstYaw;
	}

	glm::vec3 getPosition() 
	{
		return Vec3(
			sinf(PI/2 - Pitch)*sinf(Yaw),
			-cosf(PI/2 - Pitch),
			sinf(PI/2 - Pitch)*cosf(Yaw)
		);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		if (cameraState == CAM_FIXED) return;
		float ang_disp = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Pitch -= ang_disp;
		if (direction == BACKWARD)
			Pitch += ang_disp;
		if (direction == LEFT)
			Yaw += ang_disp;
		if (direction == RIGHT)
			Yaw -= ang_disp;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		if (cameraState == CAM_FIXED) return;
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw -= xoffset;
		Pitch += yoffset;
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		//if (cameraState == CAM_FIXED) return;
		Zoom -= ScrollSensitivity*yoffset;
		if (Zoom < 0.2f) Zoom = 0.2f;
		if (Zoom > 3.0f) Zoom = 3.0;
		//printf("Zoom:%.2f\n", Zoom);
	}

	glm::quat orientation() {
		return glm::quat(glm::vec3(Pitch, Yaw, 0));
	}

private:
	//glm::mat4 LookAtMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 LookAtMat = glm::lookAt(Vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 GetModelMatrix()
	{
		//model = glm::scale(model, Vec3(Zoom));
		glm::mat4 model = glm::toMat4(orientation())*glm::mat4(1.0f);
		model = glm::scale(model, Vec3(Zoom));
		return model;
	}
};
