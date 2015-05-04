#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include <iostream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Framebuffer
{
	public:
		Framebuffer(GLboolean depthBuf, GLuint width, GLuint height);
		~Framebuffer();

		void textureBuffer();
		void depthTest();
		void enable();
		void disable();
		GLuint getRenderedTexture() const;

		static void vertexQuad();
		static void drawQuad();

	protected:
		GLuint m_FramebufferName;
		GLuint m_renderedTexture;
		GLuint m_depthrenderbuffer;

		GLuint m_width, m_height;

		GLboolean m_depthBufTest;

		static GLuint quad_VertexArrayID;
		static GLuint quad_vertexbuffer;
};

#endif