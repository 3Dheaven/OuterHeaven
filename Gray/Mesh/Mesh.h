#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>

#include "../shader/shader.h"
#include "../material/Material.h"

struct Vertex {
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // TexCoords
    glm::vec2 TexCoords;
	// Tangent
	//glm::vec3 Tangent;
	// Bitangent
	//glm::vec3 Bitangent;
};

class Mesh
{
public:
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, GLuint materialIndex, aiMatrix4x4 modelMatrix);
	~Mesh();

	GLuint getMaterialIndex() const;

    // Render the mesh
    void Draw(const Shader &shader, const Material& mat) const;

	void Draw(const Shader &shader, const Material& mat, const int nbInstance) const;

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

	aiMatrix4x4 m_modelMatrix;

	GLuint m_sizeIndices;
	GLuint m_materialIndex;

    // Initializes all the buffer objects/arrays
    void setupMesh(const vector<Vertex>& vertices, const vector<GLuint>& indices);
};