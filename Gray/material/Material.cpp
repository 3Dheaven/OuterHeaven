#include "Material.h"

Material::Material(aiColor4D ka, aiColor4D kd, aiColor4D ks, GLuint texDiffuse, GLuint texNormal, GLuint texSpecular):
	m_ka(ka), m_kd(kd), m_ks(ks), m_texDiffuse(texDiffuse), m_texNormal(texNormal), m_texSpecular(texSpecular)
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

aiColor4D Material::getKa() const
{
	return m_ka;
}

aiColor4D Material::getKd() const
{
	return m_kd;
}

aiColor4D Material::getKs() const
{
	return m_ks;
}

void Material::bind(const Shader &shader) const
{
	glEnable(GL_TEXTURE_2D);

	if(m_texDiffuse)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texDiffuse);
	}

	if(m_texNormal)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_texNormal);
	}

	if(m_texSpecular)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_texSpecular);
	}

	glUniform3i(glGetUniformLocation(shader.Program, "texDNS"), m_texDiffuse, m_texNormal, m_texSpecular);

	glUniform3f(glGetUniformLocation(shader.Program, "ka"), m_ka.r, m_ka.g, m_ka.b);
	glUniform3f(glGetUniformLocation(shader.Program, "kd"), m_kd.r, m_kd.g, m_kd.b);
	glUniform3f(glGetUniformLocation(shader.Program, "ks"), m_ks.r, m_ks.g, m_ks.b);
}

void Material::unbind() const
{
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
}

Material::~Material()
{
}