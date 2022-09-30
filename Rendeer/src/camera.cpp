#include <iostream>
#include "camera.h"
#include "input/keyboard.h"
#include "input/mouse.h"

namespace rendeer
{
    Camera::Camera(
        glm::vec3 position,
        glm::vec3 up,
        float yaw,
        float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                       MovementSpeed(SPEED),
                       MouseSensitivity(SENSITIVITY),
                       Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    void Camera::Update(float deltaTime)
    {
        UpdateKeyboardInput(deltaTime);
        UpdateMouseInput();
        UpdateCameraVectors();
    }

    void Camera::UpdateKeyboardInput(float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;
        if (input::Keyboard::GetKey(RENDEER_INPUT_KEY_W))
            Position += Front * velocity;
        if (input::Keyboard::GetKey(RENDEER_INPUT_KEY_S))
            Position -= Front * velocity;
        if (input::Keyboard::GetKey(RENDEER_INPUT_KEY_D))
            Position += Right * velocity;
        if (input::Keyboard::GetKey(RENDEER_INPUT_KEY_A))
            Position -= Right * velocity;
    }

    void Camera::UpdateMouseInput()
    {
        float xOffset{(float)input::Mouse::DX()}, yOffset{(float)input::Mouse::DY()};

        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch -= yOffset;

        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;

        float scrollY = input::Mouse::ScrollY();
        if (scrollY != 0)
        {

            Zoom -= scrollY;

            if (Zoom < 1.0f)
                Zoom = 1.0f;
            if (Zoom > 45.0f)
                Zoom = 45.0f;
        }
    }

    void Camera::ResetView()
    {
        Yaw = YAW;
        Pitch = PITCH;
        WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        UpdateCameraVectors();
    }

    void Camera::UpdateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);

        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
} // namespace rendeer
