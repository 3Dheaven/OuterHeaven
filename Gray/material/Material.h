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
	Material(aiColor4D ka, aiColor4D kd, aiColor4D ks, GLuint texDiffuse, GLuint texNormal, GLuint texSpecular);
	~Material();

	GLuint getTexDiffuse() const;
	GLuint getTexNormal() const;
	GLuint getTexSpecular() const;

	aiColor4D getKa() const;
	aiColor4D getKd() const;
	aiColor4D getKs() const;

	void bind(const Shader &shader) const;
	void unbind() const;

private:
	aiColor4D m_ka, m_kd, m_ks;
	GLuint m_texDiffuse, m_texNormal, m_texSpecular;
};