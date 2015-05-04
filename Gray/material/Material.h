#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../shader/Shader.h"

class Material
{
public:
	Material(aiColor4D ambient, aiColor4D diffuse, aiColor4D specular, GLuint textureDiffuse, GLuint textureNormal, GLuint textureSpecular);
	~Material();

	GLuint getTexDiffuse() const;
	GLuint getTexNormal() const;
	GLuint getTexSpecular() const;

	aiColor4D getAmbient() const;
	aiColor4D getDiffuse() const;
	aiColor4D getSpecular() const;

	void bind(const Shader &shader) const;
	void unbind() const;

private:
	aiColor4D m_ambient, m_diffuse, m_specular;
	GLuint m_texDiffuse, m_texNormal, m_texSpecular;
};