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
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Model.h"

class Nature : public Model
{
public:
    Nature(const Camera& camera, const Engine& engine);
	~Nature();

    void Draw();
    
protected:
	Shader m_grassShader, m_treeShader;

	GLint m_grassTex1, m_grassTex2, m_grassTex3, m_grassTex4;
};