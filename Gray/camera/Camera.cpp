#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), 
	MouseSensitivity(SENSITIVTY), Zoom(ZOOM), m_update(false)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM), m_update(false)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

GLboolean Camera::needUpdate() const
{
	return m_update;
}

void Camera::setUpdate(GLboolean v)
{
	m_update = v;
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;

	m_update = true;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (this->Pitch > 89.0f)
        this->Pitch = 89.0f;
    if (this->Pitch < -89.0f)
        this->Pitch = -89.0f;

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();

	m_update = true;
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;

	m_update = true;
}

void Camera::ProcessJoystickPad(GLfloat axis1x, GLfloat axis1y, GLfloat axis2x, GLfloat axis2y, GLfloat deltaTime)
{
	if(abs(axis2y) > 0.2) this->Yaw += axis2y * MovementSpeed * deltaTime;
	if(abs(axis2x) > 0.2) this->Pitch -= axis2x * MovementSpeed * deltaTime;

	if (this->Pitch > 89.0f)
        this->Pitch = 89.0f;
    if (this->Pitch < -89.0f)
        this->Pitch = -89.0f;

	updateCameraVectors();

	if(abs(axis1y) > 0.2) this->Position -= this->Front * axis1y;
	if(abs(axis1x) > 0.2) this->Position += this->Right * axis1x;

	m_update = true;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}

GLfloat Camera::getZoom() const
{
	return Zoom;
}

glm::vec3 Camera::getPosition() const
{
	return Position;
}

glm::vec3 Camera::getFront() const
{
	return Front;
}

glm::vec3 Camera::getUp() const
{
	return Up;
}