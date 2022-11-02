#include "camera.h"

#include "application.h"

Camera::Camera(GLFWwindow* context
    , glm::vec3 position 
    , glm::vec3 up    
    , glm::vec3 front)
    : m_context(context), m_movement({position, front, glm::vec3(0.0f)})
{
    glm::vec3 right = glm::normalize(glm::cross(up, front));
    m_movement.up = glm::normalize(glm::cross(front, right));
    glfwGetCursorPos(m_context, &m_lastX, &m_lastY);

    UpdateCameraVectors();
}

void Camera::UpdateEulers(double x, double y, bool lockValues, bool constrainPitch)
{
	double offsetX = x - m_lastX;
	double offsetY = y - m_lastY;

    m_lastX = x;
    m_lastY = y;
 
	if (lockValues)
		return;
    
	offsetX *= m_sensitivity;
	offsetY *= m_sensitivity;

    float& pitch = m_eulers.pitch;
    float& yaw = m_eulers.yaw;
	yaw += static_cast<float>(offsetX);
	pitch -= static_cast<float>(offsetY);

    UpdateCameraVectors();

    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        else if (pitch < -89.0f)
            pitch = -89.0f;
    }
}

void Camera::UpdatePosition(float timeStep)
{
    const float cameraMultiplier = glfwGetKey(m_context, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ? 7.5f : 2.5f;
    const float cameraSpeed = timeStep * cameraMultiplier; 
    if (Application::IsKeyPressed(GLFW_KEY_W))
        m_movement.position += cameraSpeed * m_movement.front;
    if (Application::IsKeyPressed(GLFW_KEY_A))
        m_movement.position += glm::normalize(glm::cross(m_movement.up, m_movement.front)) * cameraSpeed;
    if (Application::IsKeyPressed(GLFW_KEY_S))
        m_movement.position -= cameraSpeed * m_movement.front;
    if (Application::IsKeyPressed(GLFW_KEY_D))
        m_movement.position -= glm::normalize(glm::cross(m_movement.up, m_movement.front)) * cameraSpeed;
}

void Camera::UpdateScroll(float y)
{
    float& fov = m_eulers.fov;
    fov -= 2*y;
    if (fov < 1.0f)
        fov = 1.0f;
    else if (fov > 90.0f)
        fov = 90.0f;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_eulers.yaw)) * cos(glm::radians(m_eulers.pitch));
    direction.y = sin(glm::radians(m_eulers.pitch));
    direction.z = sin(glm::radians(m_eulers.yaw)) * cos(glm::radians(m_eulers.pitch));
    m_movement.front = glm::normalize(direction);
}

glm::mat4 Camera::GetLookAtMatrix() const
{
    glm::vec3 target = m_movement.position + m_movement.front;
    return glm::lookAt(m_movement.position, target, m_movement.up);
}

