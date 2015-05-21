#include "Nature.h"

Nature::Nature(Camera& camera, const Engine& engine):Model("../obj/nature/nature.obj", camera, engine), 
	m_grassShader("shaders/nature/grass/vertex.glsl", "shaders/nature/grass/fragment.glsl"),
	m_treeShader("shaders/nature/tree/vertex.glsl", "shaders/nature/tree/fragment.glsl")
{
	m_grassTex1 = TextureFromFile("billboardblueflowers.png", "../obj/nature/grass");
	m_grassTex2 = TextureFromFile("billboardgrass0001.png", "../obj/nature/grass");
	m_grassTex3 = TextureFromFile("billboardgrass0002.png", "../obj/nature/grass");
	m_grassTex4 = TextureFromFile("billboardredflowers.png", "../obj/nature/grass");
}

Nature::~Nature()
{
}

void Nature::Draw()
{
	m_grassShader.Use();

	// Transformation matrices
	glm::mat4 projection = glm::perspective(m_camera.getZoom(), (GLfloat)m_engine.getScreenWidth()/(GLfloat)m_engine.getScreenHeight(), 0.1f, 5000.0f);
	glm::mat4 view = m_camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(m_grassShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_grassShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	glUniformMatrix4fv(glGetUniformLocation(m_grassShader.Program, "rotationMatrix1"), 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(1.0f), 50.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
	glUniformMatrix4fv(glGetUniformLocation(m_grassShader.Program, "rotationMatrix2"), 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(1.0f), -50.0f, glm::vec3(0.0f, 1.0f, 0.0f))));

	glShaderStorageBlockBinding(m_grassShader.Program, glGetProgramResourceIndex(m_grassShader.Program, GL_SHADER_STORAGE_BLOCK, "LightData"), 0);

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_grassTex1);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_grassTex2);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_grassTex3);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, m_grassTex4);

	m_lightShaderStorageBuffer.bind();

	this->meshes[0]->Draw(m_grassShader, m_material[this->meshes[0]->getMaterialIndex()], 3*50);

	/*
	m_treeShader.Use();

	glUniformMatrix4fv(glGetUniformLocation(m_treeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(m_treeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glUniformMatrix4fv(glGetUniformLocation(m_treeShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, -0.5f, 2.0f))));
	
	glUniformMatrix4fv(glGetUniformLocation(m_treeShader.Program, "rotationMatrix1"), 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(1.0f), 50.0f, glm::vec3(0.0f, 1.0f, 0.0f))));
	glUniformMatrix4fv(glGetUniformLocation(m_treeShader.Program, "rotationMatrix2"), 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::mat4(1.0f), -50.0f, glm::vec3(0.0f, 1.0f, 0.0f))));

	glShaderStorageBlockBinding(m_treeShader.Program, glGetProgramResourceIndex(m_treeShader.Program, GL_SHADER_STORAGE_BLOCK, "LightData"), 0);
	
	this->meshes[1].Draw(m_treeShader, m_material[this->meshes[1].getMaterialIndex()], 1);
	this->meshes[2].Draw(m_treeShader, m_material[this->meshes[2].getMaterialIndex()], 1);
	*/

	m_lightShaderStorageBuffer.release();

	glDisable(GL_TEXTURE_2D);
}