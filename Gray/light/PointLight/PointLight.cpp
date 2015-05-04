#include "PointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, 
	GLfloat constant, GLfloat linear, GLfloat quadratic):m_constant(constant), m_linear(linear), m_quadratic(quadratic)
{
	Light::Light(position, ambient, diffuse, specular);
}

GLfloat PointLight::getConstant() const
{
	return m_constant;
}

GLfloat PointLight::getLinear() const
{
	return m_linear;
}

GLfloat PointLight::getQuadratic() const
{
	return m_quadratic;
}

PointLight::~PointLight()
{
}