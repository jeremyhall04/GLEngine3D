#include "camera.h"
#include "../graphics/window/window.h"

Camera::Camera()
{}

Camera::Camera(glm::vec3 position, glm::vec3 direction, CameraType cameraType)
	: m_Position(position), m_Direction(direction), m_CameraType(cameraType)
{}


Camera::~Camera()
{}





OrthoCamera::OrthoCamera(glm::vec3 position, glm::vec3 direction, glm::vec2 h_limits, glm::vec2 v_limits)
	: Camera(position, direction, type_OrthoCamera), m_HorLimits(h_limits), m_VertLimits(v_limits)
{
	update();
}

OrthoCamera::OrthoCamera(float xPos, float yPos, float zPos, glm::vec3 direction, glm::vec2 h_limits, glm::vec2 v_limits)
	: Camera(glm::vec3(xPos, yPos, zPos), direction, type_OrthoCamera), m_HorLimits(h_limits), m_VertLimits(v_limits)
{
	update();
}

void OrthoCamera::update()
{
	m_ViewProj.view = glm::mat4(1.0f);
	m_ViewProj.projection = glm::ortho(m_HorLimits[0], m_HorLimits[1], m_VertLimits[0], m_VertLimits[1], -100.0f, 100.0f);
}




PerspectiveCamera::PerspectiveCamera()
{}

PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float fov)
	: Camera(position, direction, type_PerspectiveCamera), m_FOV(fov), m_Aspect((float)SCR_WIDTH / (float)SCR_HEIGHT), m_zNear(0.01f), m_zFar(100.0f)
{
	m_Pitch = 0.0f;
	m_Yaw = (atan2(direction.z, direction.x) * 180) / PI;
	update();
}

PerspectiveCamera::PerspectiveCamera(float xPos, float yPos, float zPos, glm::vec3 direction, float fov)
	: Camera(glm::vec3(xPos, yPos, zPos), direction, type_PerspectiveCamera), m_FOV(fov), m_Aspect((float)SCR_WIDTH / (float)SCR_HEIGHT), m_zNear(0.01f), m_zFar(100.0f)
{
	m_Pitch = 0.0f;
	m_Yaw = (atan2(direction.z, direction.x) * 180) / PI;
	update();
}

void PerspectiveCamera::processKeyboardInput(GLFWwindow* window, float deltaTime)
{
	float velocity = m_CameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_Position += m_CameraSpeed * m_Direction;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_Position -= m_CameraSpeed * m_Direction;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_Position -= glm::normalize(glm::cross(m_Direction, m_Up)) * m_CameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_Position += glm::normalize(glm::cross(m_Direction, m_Up)) * m_CameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		m_Position.y += m_CameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		m_Position.y -= m_CameraSpeed;
}

void PerspectiveCamera::processKeyboardInput(CameraMovement direction, float deltaTime)
{
	float velocity = m_CameraSpeed * deltaTime;
	if (direction == FORWARD)
		m_Position += glm::vec3(m_Direction.x * velocity, 0.0f, m_Direction.z * velocity);
	if (direction == BACKWARD)
		m_Position -= glm::vec3(m_Direction.x * velocity, 0.0f, m_Direction.z * velocity);
	if (direction == LEFT)
		m_Position -= glm::vec3(m_Right.x * velocity, 0.0f, m_Right.z * velocity);
	if (direction == RIGHT)
		m_Position += glm::vec3(m_Right.x * velocity, 0.0f, m_Right.z * velocity);
	if (direction == UP)
		m_Position.y += velocity;
	if (direction == DOWN)
		m_Position.y -= velocity;

	//printf("\n%f, %f", m_Position.x, m_Position.y, m_Position.z);

}

/*void PerspectiveCamera::processMouseMovement(GLFWwindow* window, double xpos, double ypos)
{
	if (m_FirstMouse)
	{
		m_MouseLastX = (float)SCR_WIDTH / 2.0f;
		m_MouseLastY = (float)SCR_HEIGHT / 2.0f;
		glfwSetCursorPos(window, m_MouseLastX, m_MouseLastY);
		m_FirstMouse = false;
	}

	float xOffset = xpos - m_MouseLastX;
	float yOffset = m_MouseLastY - ypos;

	if (xOffset > 1.0f || xOffset < -1.0f)
	{
		m_MouseLastX = xpos;
		xOffset *= m_MouseSensitivity;
		m_Yaw += xOffset;
	}
	if (yOffset > 1.0f || yOffset < -1.0f)
	{
		m_MouseLastY = ypos;
		yOffset *= m_MouseSensitivity;
		m_Pitch += yOffset;
	}
	// make sure that when pitch is out of bounds, screen doesn't get flipped

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	// update Direction, Right and Up Vectors using the updated Euler angles
	update();
}*/

void PerspectiveCamera::processMouseMovement(float xOffset, float yOffset)
{
	m_Yaw += xOffset;
	m_Pitch += yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	// update Direction, Right and Up Vectors using the updated Euler angles
	//update();
}


void PerspectiveCamera::update()
{
	glm::vec3 dir;
	float pitchR = glm::radians(m_Pitch);
	float yawR = glm::radians(m_Yaw);
	
	dir.x = cos(yawR) * cos(pitchR);
	dir.y = sin(pitchR);
	dir.z = sin(yawR) * cos(pitchR);
	//printf("\ndir = <%f, %f, %f>", dir.x, dir.y, dir.z);

	m_Direction = glm::normalize(dir);
	m_Right = glm::normalize(glm::cross(m_Direction, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Direction));
	m_ViewProj.view = glm::lookAt(m_Position, m_Position + m_Direction, m_Up);
	m_ViewProj.projection = glm::perspective(m_FOV, m_Aspect, m_zNear, m_zFar);
	//printf("\nPosition = <%f, %f, %f>", m_Position.x, m_Position.y, m_Position.z);
}
