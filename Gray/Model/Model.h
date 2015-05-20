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

#include "../engine/engine.h"
#include "../mesh/Mesh.h"
#include "../material\Material.h"
#include "../camera/camera.h"

#include "../Buffer/GPUBuffer.h"

struct Light
{
	glm::vec4 position;
	glm::vec4 ambient, diffuse, specular;
	glm::vec4 attenuation;
	glm::vec4 cutOff;
	glm::vec4 direction;
};

struct Matrices
{
	glm::mat4 model, view, projection, normal;
};

Light createLight(aiVector3D position, aiColor3D ambient, aiColor3D diffuse, aiColor3D specular, GLfloat constant, GLfloat linear, 
				  GLfloat quadratic, GLfloat angleInnerCone, GLfloat angleOuterCone, aiVector3D direction);

class Model
{
public:
    Model(GLchar* path, Camera& camera, const Engine& engine);

    virtual void Draw();
    
protected:
    /*  Model Data  */
	vector<Material> m_material;
    vector<Mesh> meshes;
    string directory;

	Camera& m_camera;
	const Engine& m_engine;

	Shader m_shader;

	GPUBuffer m_lightShaderStorageBuffer, m_matricesSSBO;

    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path);

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 modelMatrix);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 modelMatrix) const;

	void processMaterial(const aiScene* scene);
	void processLight(const aiScene* scene);

	void processMatrices();
	void updateMatrices();
	Matrices createMatrices() const;

	GLint TextureFromFile(const char* path, string directory);
};