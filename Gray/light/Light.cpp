#include "Light.h"

Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular):
	m_position(position), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
{
}

glm::vec3 Light::getPosition() const
{
	return m_position;
}

glm::vec3 Light::getAmbient() const
{
	return m_ambient;
}

glm::vec3 Light::getDiffuse() const
{
	return m_diffuse;
}

glm::vec3 Light::getSpecular() const
{
	return m_specular;
}

Light::~Light()
{
}