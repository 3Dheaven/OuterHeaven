#include "framebuffer.h"

GLuint Framebuffer::quad_VertexArrayID;
GLuint Framebuffer::quad_vertexbuffer;

Framebuffer::Framebuffer(GLboolean depthBufTest, GLuint width, GLuint height):m_width(width), m_height(height), m_depthBufTest(depthBufTest)
{
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	m_FramebufferName = 0;
	glGenFramebuffers(1, &m_FramebufferName);

	textureBuffer();
	if(m_depthBufTest) depthTest();
}

GLuint Framebuffer::getRenderedTexture() const
{
	return m_renderedTexture;
}

void Framebuffer::vertexQuad()
{
	// The fullscreen quad's FBO
	glGenVertexArrays(1, &quad_VertexArrayID);
	glBindVertexArray(quad_VertexArrayID);
 
	static const GLfloat g_quad_vertex_buffer_data[18] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
	};
 
	glGenBuffers(1, &quad_vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void Framebuffer::drawQuad()
{
	glBindVertexArray(quad_VertexArrayID);
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vertexbuffer);
	glVertexAttribPointer(
		0,// attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,// size
		GL_FLOAT,// type
		GL_FALSE,// normalized?
		0,// stride
		(void*)0// array buffer offset
	);
	glDrawArrays(GL_TRIANGLES, 0, 3*2); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);
}

void Framebuffer::textureBuffer()
{
	// The texture we're going to render to
	glGenTextures(1, &m_renderedTexture);
 
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, m_renderedTexture);
 
	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
}

void Framebuffer::depthTest()
{
	// The depth buffer
	glGenRenderbuffers(1, &m_depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
}

void Framebuffer::enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferName);

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_renderedTexture, 0);
	if(m_depthBufTest) glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthrenderbuffer);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Framebuffer::disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::~Framebuffer()
{
}