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
	float m_sensitivity = 0.1f;
	bool m_first = true;

public:
	Camera(GLFWwindow* context, glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f)
		, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f)
		, glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f));
	~Camera() = default;

	inline constexpr float Fov() const { return m_eulers.fov; }

	inline constexpr glm::vec3 Position() const { return m_movement.position; }
	
	inline constexpr glm::vec3 Front() const { return m_movement.front; }

	void UpdateEulers(double x, double y, bool lockValues, bool constrainPitch = true);

	void UpdatePosition(float timeStep);

	void UpdateScroll(float y);

	void UpdateCameraVectors();

	glm::mat4 GetLookAtMatrix() const;
};
