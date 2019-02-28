#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_transform.hpp>

#define PI 3.14159265358979323846
#define TAU 2*PI

typedef glm::vec3 Vec3;
typedef glm::quat Quat;

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
	float Radius;
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
		return glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f))*glm::inverse(model);
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
			Radius*sinf(PI/2 - Pitch)*sinf(Yaw),
			-Radius*cosf(PI/2 - Pitch),
			Radius*sinf(PI/2 - Pitch)*cosf(Yaw)
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
		if (Zoom < 0.1f) Zoom = 0.1f;
		if (Zoom > 7.0f) Zoom = 7.0;
		//printf("Zoom:%.2f\n", Zoom);
	}

private:

	glm::mat4 GetModelMatrix()
	{
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::scale(model, Vec3(Zoom));
		model = glm::rotate(model, Yaw, Vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, Pitch, Vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, Vec3(Zoom));
		return model;
	}
	glm::quat euler2quat_r(glm::vec3 eangles) {
		glm::quat qPitch = glm::angleAxis(eangles[0], glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(eangles[1], glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(eangles[2], glm::vec3(0, 0, 1));
		return qYaw *qPitch * qRoll;
	}

	float get_fixed_rotation_angle_from_2_vector(const glm::vec3& u, const glm::vec3& v)
	{
		return acos(dot(u, v) / glm::length(u) / glm::length(v));
	}

	glm::quat euler2quat_d(glm::vec3 eangles) {
		glm::radians(eangles);
		glm::quat qPitch = glm::angleAxis(eangles[0], glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(eangles[1], glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(eangles[2], glm::vec3(0, 0, 1));
		return qYaw *qPitch * qRoll;
	}

	glm::quat euler2quat_r(float pitch, float yaw, float roll) {
		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));
		return qYaw *qPitch * qRoll;
	}

	glm::quat euler2quat_d(float pitch, float yaw, float roll) {
		pitch = glm::radians(pitch);
		yaw = glm::radians(yaw);
		roll = glm::radians(roll);
		glm::quat qPitch = glm::angleAxis(pitch, glm::vec3(1, 0, 0));
		glm::quat qYaw = glm::angleAxis(yaw, glm::vec3(0, 1, 0));
		glm::quat qRoll = glm::angleAxis(roll, glm::vec3(0, 0, 1));
		return qYaw *qPitch * qRoll;
	}
};
