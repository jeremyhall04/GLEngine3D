#ifndef CAMERA_H
#define CAMERA_H

#include "../GLcommon.h"
#include "../GLcommon_math.h"

const float m_CameraSpeed = 15.0f;
const float m_MouseSensitivity = 0.1f;
static bool m_FirstMouse = true;
static float m_MouseLastX, m_MouseLastY;
	
enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum CameraType 
{
	type_OrthoCamera, type_PerspectiveCamera
};

struct ViewProjectionMatrices {
	glm::mat4 view, perspective;
};

//-----------------------------------------------------------------------------------------------------
//-------------------------------------------- CAMERA -------------------------------------------------
//-----------------------------------------------------------------------------------------------------
class Camera
{
protected:
	ViewProjectionMatrices m_ViewProj;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Up;
	glm::vec3 m_Right;
	const glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	CameraType m_CameraType;

public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 direction, CameraType cameraType);
	~Camera();

	//glm::mat4 getViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Direction, m_Up); };
	virtual void update() {}
	glm::mat4 getViewMatrix() { return m_ViewProj.view; };
	glm::mat4 getProjectionMatrix() { return m_ViewProj.perspective; };
	const glm::vec3& getPosition() { return m_Position; };
	const glm::vec3& getDirection() { return m_Direction; };
	const glm::vec3& getRight() { return m_Right; };
	const glm::vec3& getUp() { return m_Up; };
};

//-----------------------------------------------------------------------------------------------------
//------------------------------------------- Orthographic --------------------------------------------
//-----------------------------------------------------------------------------------------------------
class OrthoCamera : public Camera
{
private:
	glm::vec2 m_HorLimits, m_VertLimits;
public:
	OrthoCamera(glm::vec3 position, glm::vec3 direction, glm::vec2 hlimits, glm::vec2 vlimits);
	OrthoCamera(float xPos, float yPos, float zPos, glm::vec3 direction, glm::vec2 hlimits, glm::vec2 vlimits);

	void update() override;
};

//-----------------------------------------------------------------------------------------------------
//------------------------------------------- Perspective ---------------------------------------------
//-----------------------------------------------------------------------------------------------------
class PerspectiveCamera : public Camera
{
private:
	float m_FOV, m_Aspect, m_zNear, m_zFar;
	float m_Pitch, m_Yaw;
public:
	PerspectiveCamera();
	PerspectiveCamera(glm::vec3 position, glm::vec3 direction, float fov);
	PerspectiveCamera(float xPos, float yPos, float zPos, glm::vec3 direction, float fov);

	void processKeyboardInput(GLFWwindow* window, float deltaTime);
	void processKeyboardInput(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset);
	void update() override;
	const glm::vec4& getPerspectiveParams() const { return glm::vec4(m_FOV, m_Aspect, m_zNear, m_zFar); };
	void updateAspect(float aspect);
};

#endif // !CAMERA_H