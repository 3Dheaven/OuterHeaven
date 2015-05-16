#pragma once

#include <iostream>
#include <string>

using namespace std;

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class GPUBuffer
{
public:
	GPUBuffer();
	~GPUBuffer();

	void create(GLint size,GLenum type, GLenum usage);
	void destroy();

	GLuint getBuffer();
	GLenum getType();

	virtual void bind();
	virtual void setBindingPoint(int bindingPoint);

	virtual void bind(int bindingPoint);
	virtual void release();

private:
	GLuint m_Buffer, m_Type, m_Size, m_BindingPoint;

};