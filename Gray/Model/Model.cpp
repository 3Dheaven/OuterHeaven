#include "Model.h"

Model::Model(GLchar* path, Camera& camera, const Engine& engine):m_lightShaderStorageBuffer(), m_camera(camera), m_engine(engine),
	m_shader("shaders/lighting/vertex.glsl", "shaders/lighting/fragment.glsl")
{
    this->loadModel(path);
}

Model::~Model()
{
	for(vector<Mesh*>::iterator it = meshes.begin(); it < meshes.end(); it++) delete *it;
}

void Model::Draw()
{
	m_shader.Use();

    for(GLuint i = 0; i < this->meshes.size(); i++)
        this->meshes[i]->Draw(m_shader, m_material[this->meshes[i]->getMaterialIndex()]);

	if(m_camera.needUpdate()) updateMatrices();
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

	this->processLight(scene);

    // Process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene, modelMatrix);

	processMatrices();

	m_lightShaderStorageBuffer.bind();
	m_matricesSSBO.bind();
}

Matrices Model::createMatrices() const
{
	Matrices mat;

	mat.model		= glm::scale(glm::mat4(1.0f), glm::vec3(0.05f));
	mat.view		= m_camera.GetViewMatrix();
	mat.projection	= glm::perspective(m_camera.getZoom(), (GLfloat)m_engine.getScreenWidth()/(GLfloat)m_engine.getScreenHeight(), 0.1f, 500.0f);
	mat.normal		= glm::transpose(glm::inverse(mat.view * mat.model));
	
	return mat;
}

void Model::processMatrices()
{
	Matrices mat = createMatrices();

	m_matricesSSBO.create(sizeof(Matrices), GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(Matrices), &mat);
	m_matricesSSBO.setBindingPoint(1);
}

void Model::updateMatrices()
{
	Matrices mat = createMatrices();

	m_matricesSSBO.bind();
	m_matricesSSBO.mapBuffer(&mat, sizeof(Matrices));

	m_camera.setUpdate(false);
}

Light createLight(aiVector3D position, aiColor3D ambient, aiColor3D diffuse, aiColor3D specular, GLfloat constant, GLfloat linear, 
				  GLfloat quadratic, GLfloat angleInnerCone, GLfloat angleOuterCone, aiVector3D direction)
{
	Light light;

	light.position = glm::vec4(position.x, position.y, position.z, 1.0f);
	light.ambient = glm::vec4(ambient.r, ambient.g, ambient.b, 1.0f);
	light.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, 1.0f);
	light.specular = glm::vec4(specular.r, specular.g, specular.b, 1.0f);
	light.attenuation = glm::vec4(constant, linear, quadratic, 1.0);
	light.cutOff = glm::vec4(angleInnerCone, angleOuterCone, 1.0f, 1.0f);
	light.direction = glm::vec4(direction.x, direction.y, direction.z, 1.0f);

	return light;
}

void Model::processLight(const aiScene* scene)
{
	vector<Light> listPointLight;
	vector<Light> listSpotLight;
	vector<Light> listDirectionalLight;

	aiLight* light;
	Light myLight;

	/*
	listPointLight.push_back(createLight(aiVector3D(-50.0f, 10.0f, 0.0f), aiColor3D(1.0f, 1.0f, 1.0f), aiColor3D(1.0f, 1.0f, 1.0f),
		aiColor3D(1.0f, 1.0f, 1.0f), 1.0f, 0.009f, 0.0032f, 0.71f, 0.52f, aiVector3D(-1.0f, -1.0f, 1.0f)));
	*/
	listPointLight.push_back(createLight(aiVector3D(0.0f, 10.0f, 0.0f), aiColor3D(1.0f, 1.0f, 1.0f), aiColor3D(1.0f, 1.0f, 1.0f),
		aiColor3D(1.0f, 1.0f, 1.0f), 1.0f, 0.009f, 0.0032f, 0.71f, 0.52f, aiVector3D(-1.0f, -1.0f, 1.0f)));
	/*
	listPointLight.push_back(createLight(aiVector3D(50.0f, 10.0f, 0.0f), aiColor3D(1.0f, 1.0f, 1.0f), aiColor3D(1.0f, 1.0f, 1.0f),
		aiColor3D(1.0f, 1.0f, 1.0f), 1.0f, 0.009f, 0.0032f, 0.71f, 0.52f, aiVector3D(0.0f, -1.0f, 0.0f)));
	*/

	for(int i=0; scene->mNumLights; i++)
	{
		light = scene->mLights[i];

		myLight = createLight(light->mPosition, light->mColorAmbient, light->mColorDiffuse, light->mColorSpecular, 
			light->mAttenuationConstant, light->mAttenuationLinear, light->mAttenuationQuadratic,
			light->mAngleInnerCone, light->mAngleOuterCone, light->mDirection);

		if(light->mType == aiLightSourceType::aiLightSource_POINT)
			listPointLight.push_back(myLight);

		if(light->mType == aiLightSourceType::aiLightSource_SPOT)
			listSpotLight.push_back(myLight);

		if(light->mType == aiLightSourceType::aiLightSource_DIRECTIONAL)
			listDirectionalLight.push_back(myLight);
	}

	GLuint nbLights = listPointLight.size() + listSpotLight.size() + listDirectionalLight.size();

	m_lightShaderStorageBuffer.create(sizeof(glm::ivec4) + sizeof(Light)*nbLights, GL_SHADER_STORAGE_BUFFER, GL_STATIC_DRAW);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::ivec4), glm::value_ptr(glm::ivec4(listPointLight.size(), listSpotLight.size(), listDirectionalLight.size(), 1)));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4), sizeof(Light)*listPointLight.size(), &listPointLight[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) + sizeof(Light)*listPointLight.size(), sizeof(Light)*listSpotLight.size(), &listSpotLight[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::ivec4) + sizeof(Light)*listPointLight.size() + sizeof(Light)*listSpotLight.size(), sizeof(Light)*listDirectionalLight.size(), &listDirectionalLight[0]);
	m_lightShaderStorageBuffer.setBindingPoint(0);
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
        this->processNode(node->mChildren[i], scene, node->mTransformation);
}

Mesh* Model::processMesh(aiMesh* mesh, const aiScene* scene, aiMatrix4x4 modelMatrix) const
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
	return new Mesh(vertices, indices, mesh->mMaterialIndex, modelMatrix.Transpose());
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

GLint Model::TextureFromFile(const char* path, string directory)
{
     //Generate texture ID and load texture data 
    string filename = string(path);
    filename = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width,height;
    unsigned char* image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);

    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    SOIL_free_image_data(image);

	glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}