#ifndef CAMERA
#define CAMERA

#include "../engine/engine.h"
#include "../transformation/transformation.h"

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

class TrackBallCamera
{
	public:
		TrackBallCamera();
		~TrackBallCamera();

		void OnMouseMotion(GLFWwindow *window, double x, double y);
		void OnMouseButton(GLFWwindow * window, int button, int action, int mods);
		void OnScrolling(GLFWwindow *window, double x, double y);
		glm::mat4 viewMatrix();
		glm::vec3 getRotatedPositionCamera();

	protected:

		double m_motionSensivity, m_scrollSensitivity;
		bool m_hold;
		float m_angleY;
		float m_angleZ;
		double m_xtmp, m_ytmp;
		float m_distance;
};

#endif