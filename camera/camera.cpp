#include "camera.h"

TrackBallCamera::TrackBallCamera(): m_hold(false), m_angleY(0.0f), m_angleZ(0.0f), m_motionSensivity(120.0), 
									m_xtmp(0.0), m_ytmp(0.0), m_scrollSensitivity(1.0), m_distance(300.0f)
{
}

void TrackBallCamera::OnScrolling(GLFWwindow *window, double x, double y)
{
	if(y > 0) m_distance -= 5.0f;
	else m_distance += 5.0f;
}

void TrackBallCamera::OnMouseMotion(GLFWwindow *window, double x, double y)
{
	x =  (2 * x - WINDOW_WIDTH ) / WINDOW_WIDTH;
    y = (2 * y - WINDOW_HEIGHT) / WINDOW_HEIGHT;

    if (m_hold) //si nous maintenons le bouton gauche enfoncé
    {
		double xrel = x - m_xtmp;
		double yrel = y - m_ytmp;

        m_angleY += (float) (-xrel * m_motionSensivity); //mouvement sur X de la souris -> changement de la rotation horizontale
        m_angleZ += (float) (yrel * m_motionSensivity); //mouvement sur Y de la souris -> changement de la rotation verticale

		if (m_angleZ >= 89.0f)
            m_angleZ = 89.0f;
        else if (m_angleZ <= -89.0f)
            m_angleZ = -89.0f;
    }

	m_xtmp = x;
	m_ytmp = y;
}

void TrackBallCamera::OnMouseButton(GLFWwindow * window, int button, int action, int mods)
{
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) m_hold = true;
	else m_hold = false;
}

glm::vec3 TrackBallCamera::getRotatedPositionCamera()
{
	glm::vec4 pos = glm::vec4(m_distance,0,0,1);
	glm::vec4 rotated_pos = Transformation::rotate( m_angleY, glm::vec3(0.0f, 1.0f, 0.0f) ) * Transformation::rotate( m_angleZ, glm::vec3(0.0f, 0.0f, 1.0f) ) * pos;
	return glm::vec3(rotated_pos.x, rotated_pos.y, rotated_pos.z);
}

glm::mat4 TrackBallCamera::viewMatrix()
{
	glm::vec3 rotated_pos = getRotatedPositionCamera();

	// Camera matrix
	glm::mat4 View       = glm::lookAt(
		glm::vec3(rotated_pos.x, rotated_pos.y, rotated_pos.z), // Camera is at (...,...,...) in World Space
		glm::vec3(0,0,0), // and looks at the origin
		glm::vec3(0,1,0)  // Head is up
	);

	return View;
}

TrackBallCamera::~TrackBallCamera()
{
}