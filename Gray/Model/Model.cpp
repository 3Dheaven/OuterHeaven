#include "Model.h"

Model::Model(GLchar* path)
{
    this->loadModel(path);
}

void Model::Draw(const Shader &shaders) const
{
    for(GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i].Draw(shaders, m_material[this->meshes[i].getMaterialIndex()]);
}

// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(string path)
{
    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }
    // Retrieve the directory path of the filepath
    this->directory = path.substr(0, path.find_last_of('/'));

	aiMatrix4x4 modelMatrix;

	this->processMaterial(scene);

    // Process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene, modelMatrix);
}

// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene, aiMatrix4x4 modelMatrix)
{
    // Process each mesh located at the current node
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // The node object only contains indices to index the actual objects in the scene. 
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; 
        this->meshes.push_back(this->processMesh(mesh, scene, node->mTransformation));	
    }

    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, node->mTransformation);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 modelMatrix) const
{
    // Data to fill
    vector<Vertex> vertices;
    vector<GLuint> indices;

    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x; 
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }
    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
   
    // Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, mesh->mMaterialIndex, modelMatrix.Transpose());
}

void Model::processMaterial(const aiScene* scene)
{
	aiColor4D ka, kd, ks;
	GLuint texDiffuse, texNormal, texSpecular;
	aiString str;

	for(GLuint i = 0; i < scene->mNumMaterials; i++)
	{
		if(AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &str))
			texDiffuse = TextureFromFile(str.C_Str(), this->directory);
		else
			texDiffuse = 0;

		if(AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_HEIGHT, 0, &str))
			texNormal = TextureFromFile(str.C_Str(), this->directory);
		else
			texNormal = 0;

		if(AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, 0, &str))
			texSpecular = TextureFromFile(str.C_Str(), this->directory);
		else
			texSpecular = 0;

		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_AMBIENT, &ka);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_DIFFUSE, &kd);
		aiGetMaterialColor(scene->mMaterials[i], AI_MATKEY_COLOR_SPECULAR, &ks);
			
		m_material.push_back(Material(ka, kd, ks, texDiffuse, texNormal, texSpecular));
	}
}

GLint TextureFromFile(const char* path, string directory)
{
     //Generate texture ID and load texture data 
    string filename = string(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);

    return textureID;
}