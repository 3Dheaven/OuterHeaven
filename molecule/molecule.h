#ifndef MOLECULE
#define MOLECULE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../object/object.h"
#include "../camera/camera.h"
#include "../texture/texture.h"
#include "../transformation/transformation.h"

class Molecule
{
	public:
		Molecule(GLuint programID, TrackBallCamera* trackBallCamera, GLFWwindow* window);
		~Molecule();
		
		void initProjectionAndUniforms();
		void setShader(GLuint shaders);
		void setCamera(TrackBallCamera* camera);
		void setWindow(GLFWwindow* window);
		void uniformVariables();

		void event();
		void animation();

		void displayAtomH1();
		void displayAtomH2();
		void displayAtomO();
		void displayLink1();
		void displayLink2();
		void display();

		void materialH();
		void materialO();
		void materialLink();

		void matrix(glm::mat4 modelMatrix);

	protected:
		Object m_sphere;
		Object m_cylinder;

		GLuint m_shaders;

		GLuint m_viewMatrixID;
		GLuint m_modelViewMatrixID;
		GLuint m_modelViewProjectionMatrixID;
		GLuint m_normalMatrixID;

		GLuint m_lightPositionID;
		GLuint m_viewPositionID;
		GLuint m_modelMatrixID;
		GLuint m_kaID;
		GLuint m_kdID;
		GLuint m_ksID;

		GLboolean m_lightAnimation;
		GLfloat m_lightAnimationAngle;
		
		glm::vec3 m_lightPosition;

		glm::mat4 m_modelMatrix;
		glm::mat4 m_modelViewMatrix;
		glm::mat4 m_modelViewProjectionMatrix;
		glm::mat4 m_normalMatrix;
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_viewMatrix;

		TrackBallCamera* m_camera;

		glm::vec3 m_ka;
		glm::vec3 m_kd;
		glm::vec3 m_ks;

		double m_last_time, m_current_time, m_ellapsed_time;
		GLfloat m_angleRotation;
		bool m_objectAnimation;

		GLFWwindow* m_window;

		GLfloat m_animationScale;
};

#endif