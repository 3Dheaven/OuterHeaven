#pragma once

#include <iostream>
using namespace std;

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default Light values
const glm::vec3 AMBIENT  = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 DIFFUSE  = glm::vec3(1.0f, 1.0f, 1.0f);
const glm::vec3 SPECULAR = glm::vec3(1.0f, 1.0f, 1.0f);

class Light
{
public:
	Light(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3 ambient = AMBIENT, 
		glm::vec3 diffuse = DIFFUSE, glm::vec3 specular = SPECULAR);

	~Light();

	glm::vec3 getPosition() const;
	glm::vec3 getAmbient() const;
	glm::vec3 getDiffuse() const;
	glm::vec3 getSpecular() const;

protected:
	glm::vec3 m_position;
	glm::vec3 m_ambient, m_diffuse, m_specular;
};