#include "molecule.h"

Molecule::Molecule(GLuint programID, TrackBallCamera* trackBallCamera, GLFWwindow* window):m_sphere("../obj/sphere.obj"), 
	m_cylinder("../obj/cylinder.obj"), m_angleRotation(0.0f), m_objectAnimation(true), m_animationScale(0.0f), 
	m_lightAnimation(false), m_lightAnimationAngle(0.0f), m_shaders(programID), m_camera(trackBallCamera), m_window(window)
{
	m_last_time = glfwGetTime();
	initProjectionAndUniforms();
}

void Molecule::setShader(GLuint shaders)
{
	m_shaders = shaders;
}

void Molecule::setCamera(TrackBallCamera* camera)
{
	m_camera = camera;
}

void Molecule::setWindow(GLFWwindow* window)
{
	m_window = window;
}

void Molecule::event()
{
	if(glfwGetKey(m_window, GLFW_KEY_A ) == GLFW_PRESS) m_objectAnimation = true;
	else if(glfwGetKey(m_window, GLFW_KEY_Z ) == GLFW_PRESS) m_objectAnimation = false;

	if(glfwGetKey(m_window, GLFW_KEY_Q ) == GLFW_PRESS) m_lightAnimation = true;
	else if(glfwGetKey(m_window, GLFW_KEY_S ) == GLFW_PRESS) m_lightAnimation = false;
}

void Molecule::animation()
{
	m_current_time = glfwGetTime();
	m_ellapsed_time = m_current_time - m_last_time;
	m_last_time = m_current_time;

	if(m_objectAnimation) m_angleRotation += 45.0f * (float)m_ellapsed_time;
	if(m_animationScale < 1.0f && m_objectAnimation) m_animationScale += 0.5f * (float)m_ellapsed_time;
	if(m_lightAnimation) m_lightAnimationAngle += 45.0f * (float)m_ellapsed_time;

	m_lightPosition = glm::vec3( Transformation::rotate(m_lightAnimationAngle, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(1000.0f, 500.0f, 400.0f, 1.0f) );
	m_viewMatrix = m_camera->viewMatrix();
}

void Molecule::matrix(glm::mat4 modelMatrix)
{
	m_modelViewMatrix = m_viewMatrix * modelMatrix;
	m_modelViewProjectionMatrix = m_projectionMatrix * m_modelViewMatrix;
	m_normalMatrix = glm::transpose( glm::inverse( m_modelViewMatrix ) );
}

void Molecule::displayAtomH1()
{
	materialH();

	matrix( Transformation::rotate(m_angleRotation, glm::vec3(0.0, 1.0, 0.0)) * Transformation::translate(glm::vec3(0.0f, -30.0f, 70.0f))
		* Transformation::scale(glm::vec3(0.6f*m_animationScale, 0.6f*m_animationScale, 0.6f*m_animationScale)) );

	uniformVariables();

	m_sphere.display();
}

void Molecule::displayAtomH2()
{
	materialH();

	matrix( Transformation::rotate(m_angleRotation, glm::vec3(0.0, 1.0, 0.0)) * Transformation::translate(glm::vec3(0.0f, -30.0f, -70.0f)) 
		* Transformation::scale(glm::vec3(0.6f*m_animationScale, 0.6f*m_animationScale, 0.6f*m_animationScale)) );

	uniformVariables();

	m_sphere.display();
}

void Molecule::displayAtomO()
{
	materialO();

	matrix( Transformation::translate(glm::vec3(0.0f, 60.0f, 0.0f)) * Transformation::scale(glm::vec3(m_animationScale, m_animationScale, m_animationScale)) );

	uniformVariables();

	m_sphere.display();
}

void Molecule::displayLink1()
{
	materialLink();

	matrix( Transformation::rotate(m_angleRotation, glm::vec3(0.0, 1.0, 0.0)) * Transformation::translate(glm::vec3(0.0f, -15.0f, -60.0f)) * Transformation::rotate( 36.0f, glm::vec3(1.0, 0.0, 0.0) ) 
		* Transformation::scale(glm::vec3(1.0f*m_animationScale, 1.7f*m_animationScale, 1.0f*m_animationScale)) );

	uniformVariables();

	m_cylinder.display();
}

void Molecule::displayLink2()
{
	materialLink();

	matrix( Transformation::rotate(m_angleRotation, glm::vec3(0.0, 1.0, 0.0)) * Transformation::translate(glm::vec3(0.0f, -15.0f, 60.0f)) * Transformation::rotate( -36.0f, glm::vec3(1.0, 0.0, 0.0) ) 
		* Transformation::scale(glm::vec3(1.0f*m_animationScale, 1.7f*m_animationScale, 1.0f*m_animationScale)) );

	uniformVariables();

	m_cylinder.display();
}

void Molecule::materialH()
{
	m_ka = glm::vec3(0.03, 0.03, 0.03);
	m_kd = glm::vec3(0.2745, 0.4275, 0.5725);
	m_ks = glm::vec3(1.0, 1.0, 1.0);
}

void Molecule::materialO()
{
	m_ka = glm::vec3(0.1, 0.0, 0.0);
	m_kd = glm::vec3(0.5, 0.0, 0.0);
	m_ks = glm::vec3(1.0, 1.0, 1.0);
}

void Molecule::materialLink()
{
	m_ka = glm::vec3(0.1, 0.1, 0.1);
	m_kd = glm::vec3(0.8, 0.8, 0.8);
	m_ks = glm::vec3(1.0, 1.0, 1.0);
}

void Molecule::display()
{
	glUseProgram(m_shaders);

	event();
	animation();

	displayAtomH1();
	displayAtomH2();
	displayAtomO();

	displayLink1();
	displayLink2();
}

void Molecule::initProjectionAndUniforms()
{
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_projectionMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 500.0f);

	m_viewMatrixID = glGetUniformLocation(m_shaders, "viewMatrix");
	m_modelViewMatrixID = glGetUniformLocation(m_shaders, "modelViewMatrix");
	m_modelViewProjectionMatrixID = glGetUniformLocation(m_shaders, "modelViewProjectionMatrix");
	m_normalMatrixID = glGetUniformLocation(m_shaders, "normalMatrix");
	m_lightPositionID = glGetUniformLocation(m_shaders, "lightPosition");

	m_kaID = glGetUniformLocation(m_shaders, "ka");
	m_kdID = glGetUniformLocation(m_shaders, "kd");
	m_ksID = glGetUniformLocation(m_shaders, "ks");
}

void Molecule::uniformVariables()
{
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
	glUniformMatrix4fv(m_modelViewMatrixID, 1, GL_FALSE, &m_modelViewMatrix[0][0]);
	glUniformMatrix4fv(m_modelViewProjectionMatrixID, 1, GL_FALSE, &m_modelViewProjectionMatrix[0][0]);
	glUniformMatrix4fv(m_normalMatrixID, 1, GL_FALSE, &m_normalMatrix[0][0]);

	glUniform3fv(m_kaID, 1, &m_ka.x);
	glUniform3fv(m_kdID, 1, &m_kd.x);
	glUniform3fv(m_ksID, 1, &m_ks.x);

	glUniform3fv(m_lightPositionID, 1, &m_lightPosition.x);
}

Molecule::~Molecule()
{
}