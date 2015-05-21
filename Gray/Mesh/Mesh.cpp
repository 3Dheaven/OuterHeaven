#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, GLuint materialIndex, aiMatrix4x4 modelMatrix):
	m_modelMatrix(modelMatrix), m_materialIndex(materialIndex)
{
	m_sizeIndices = indices.size();
    this->setupMesh(vertices, indices);
}

GLuint Mesh::getMaterialIndex() const
{
	return m_materialIndex;
}

// Render the mesh
void Mesh::Draw(const Shader &shader, const Material& mat) const
{
	mat.bind(shader);

    // Draw mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, m_sizeIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

	mat.unbind();
}

void Mesh::Draw(const Shader &shader, const Material& mat, const int nbInstance) const
{
	//glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, &m_modelMatrix[0][0]);

	mat.bind(shader);

    // Draw mesh
    glBindVertexArray(this->VAO);
	glDrawElementsInstanced(GL_TRIANGLES, m_sizeIndices, GL_UNSIGNED_INT, 0, nbInstance);
    glBindVertexArray(0);

	mat.unbind();
}

// Initializes all the buffer objects/arrays
void Mesh::setupMesh(const vector<Vertex>& vertices, const vector<GLuint>& indices)
{
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
	glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW); 

	// Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);	
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // Vertex Normals
    glEnableVertexAttribArray(1);	
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));

    // Vertex Texture Coords
    glEnableVertexAttribArray(2);	
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	/*
	// Vertex Tangents
    glEnableVertexAttribArray(3);	
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));

	// Vertex Bitangents
    glEnableVertexAttribArray(4);	
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));
	*/

	glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}