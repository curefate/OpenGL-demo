#pragma once
#ifndef CAMERA_FOLLOW_H
#define CAMERA_FOLLOW_H
#include "Camera.h"
#include "Object.h"

class Camera_Follow : public Camera
{
public:
    glm::vec3 direction;

    Camera_Follow()
    {
        target = glm::vec3(0);
        distance = 2.0f;

        Position = glm::vec3(target.x + distance, target.y + distance, target.z + distance);
        WorldUp = glm::vec3(0, 1, 0);
        Yaw = -90.0f;
        Pitch = 0;
        Front = glm::vec3(0, 0, -1);
        MovementSpeed = 2.5f;
        MouseSensitivity = SENSITIVITY;
        Zoom = ZOOM;
        updateCameraVectors();
    }
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, target, Up);
    }
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch -= yoffset;

        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
	}
	void ProcessMouseScroll(float yoffset)
    {
        distance -= (float)yoffset * SENSITIVITY;
        if (distance < 0.5f)
            distance = 0.5f;
        if (distance > 5.0f)
            distance = 5.0f;
    }
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
    }
    void update(glm::vec3 target)
    {
        this->target = target;
        updateCameraVectors();
    }
private:
    glm::vec3 target;
    float distance;
    void updateCameraVectors()
    {
        glm::vec3 position;
        position.x = target.x + distance * cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        position.y = target.y + distance * sin(glm::radians(Pitch));
        position.z = target.z + distance * cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Position = position;

        direction = target - Position;

        Right = glm::normalize(glm::cross(direction, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, direction));
    }
};

#endif // !CAMERA_FOLLOW_H
