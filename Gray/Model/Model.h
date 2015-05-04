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

#include "../mesh/Mesh.h"
#include "../material\Material.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
    Model(GLchar* path);

    void Draw(const Shader &shaders) const;
    
private:
    /*  Model Data  */
	vector<Material> m_material;
    vector<Mesh> meshes;
    string directory;

    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string path);

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 modelMatrix);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 modelMatrix) const;

	void processMaterial(const aiScene* scene);
};