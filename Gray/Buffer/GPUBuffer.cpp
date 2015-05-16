#include "GPUBuffer.h"

GPUBuffer::GPUBuffer()
{
}

GPUBuffer::~GPUBuffer()
{
	destroy();
}

void GPUBuffer::destroy()
{
	glDeleteBuffers(1,&m_Buffer);
}

void GPUBuffer::create(GLint size,GLenum type, GLenum usage)
{
	m_Type = type;

	m_Size = size;
	glGenBuffers(1, &m_Buffer);
	glBindBuffer(type, m_Buffer);
	if (size != 0)
		glBufferData(type, size , NULL, usage);
	m_BindingPoint = -1;
}

GLuint GPUBuffer::getBuffer()
{
	return m_Buffer;
}

GLenum GPUBuffer::getType()
{
	return m_Type;
}

void GPUBuffer::bind()
{
	glBindBufferBase(m_Type,m_BindingPoint,m_Buffer);
	/*
	if (m_BindingPoint > 0)	glBindBufferBase(m_Type,m_BindingPoint,m_Buffer);
	else glBindBuffer(m_Type, m_Buffer);
	*/
}

void GPUBuffer::bind(int bindingPoint)
{
	m_BindingPoint = bindingPoint;
	glBindBufferBase(m_Type, m_BindingPoint, m_Buffer);
}

void GPUBuffer::release()
{
	glBindBufferBase(m_Type,m_BindingPoint,0);
}

void GPUBuffer::setBindingPoint(int bindingPoint)
{
	m_BindingPoint = bindingPoint;
}