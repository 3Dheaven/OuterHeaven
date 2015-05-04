#include "Material.h"

Material::Material(aiColor4D ambient, aiColor4D diffuse, aiColor4D specular, GLuint textureDiffuse, GLuint textureNormal, 
	GLuint textureSpecular):m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_texDiffuse(textureDiffuse), 
	m_texNormal(textureNormal), m_texSpecular(textureSpecular)
{
}

GLuint Material::getTexDiffuse() const
{
	return m_texDiffuse;
}

GLuint Material::getTexNormal() const
{
	return m_texNormal;
}

GLuint Material::getTexSpecular() const
{
	return m_texSpecular;
}

aiColor4D Material::getAmbient() const
{
	return m_ambient;
}

aiColor4D Material::getDiffuse() const
{
	return m_diffuse;
}

aiColor4D Material::getSpecular() const
{
	return m_specular;
}

void Material::bind(const Shader &shader) const
{
	glEnable(GL_TEXTURE_2D);

	if(m_texDiffuse)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texDiffuse);
		glUniform1i(glGetUniformLocation(shader.Program, "material.texture_diffuse"), 0);
	}

	if(m_texNormal)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texNormal);
		glUniform1i(glGetUniformLocation(shader.Program, "material.texture_normal"), 1);
	}

	if(m_texSpecular)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_texSpecular);
		glUniform1i(glGetUniformLocation(shader.Program, "material.texture_specular"), 2);
	}

	glUniform3i(glGetUniformLocation(shader.Program, "material.texDNS"), m_texDiffuse, m_texNormal, m_texSpecular);

	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), m_ambient.r, m_ambient.g, m_ambient.b);
	glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), m_diffuse.r, m_diffuse.g, m_diffuse.b);
	glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), m_specular.r, m_specular.g, m_specular.b);
}

void Material::unbind() const
{
	if(m_texDiffuse)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if(m_texNormal)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	if(m_texSpecular)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glDisable(GL_TEXTURE_2D);
}

Material::~Material()
{
}