#pragma once

namespace detail
{
	class CameraEulersAttrib
	{
	public:
		float pitch = 0.0f;
		float yaw = -90.0f;
		float roll = 0.0f;
		float fov = 45.0f;
	};

	class CameraMovementAttrib
	{
	public:
		glm::vec3 position;
		glm::vec3 front;
		glm::vec3 up;
	};
};

class Camera
{
private:
	GLFWwindow* m_context;
	detail::CameraEulersAttrib m_eulers;
	detail::CameraMovementAttrib m_movement;
	double m_lastX = 0.0f;
	double m_lastY = 0.0f;

public:
	Camera(GLFWwindow* context, glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f)
		, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
		, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
	~Camera() = default;

	inline constexpr float fov() const { return m_eulers.fov; }
	inline constexpr glm::vec3 position() const { return m_movement.position; }
	inline constexpr glm::vec3 front() const { return m_movement.front; }

	void processEulers(double x, double y, bool lockValues, bool constrainPitch = true);
	void processMovement(int32_t direction, float delta);
	void processScroll(float y);
	void updateCameraVectors();
	glm::mat4 constructLookAt() const;
};
