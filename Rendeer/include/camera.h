#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

namespace rendeer
{
    class Camera
    {
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // euler angles
        float Yaw;
        float Pitch;

        // camera settings
        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW,
            float pitch  = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

        void Update(float);
        void ResetView();

        inline glm::mat4 GetViewMatrix() { return glm::lookAt(Position, Position + Front, Up); }

    private:
        void UpdateMouseInput();
        void UpdateKeyboardInput(float);
        void UpdateCameraVectors();
    };
} // namespace rendeer
