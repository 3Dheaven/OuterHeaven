#pragma once

#include "../Light.h"

// Default PointLight values
const GLfloat CONSTANT  = 1.0f;
const GLfloat LINEAR    = 0.009f;
const GLfloat QUADRATIC = 0.0032f;

class PointLight : public Light
{
public:
	PointLight(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3 ambient = AMBIENT, glm::vec3 diffuse = DIFFUSE, 
		glm::vec3 specular = SPECULAR, GLfloat constant = CONSTANT, GLfloat linear = LINEAR, GLfloat quadratic = QUADRATIC);

	~PointLight();

	GLfloat getConstant() const;
	GLfloat getLinear() const;
	GLfloat getQuadratic() const;

private:
	GLfloat m_constant, m_linear, m_quadratic;
};