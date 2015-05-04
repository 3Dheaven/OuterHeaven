#ifndef ENGINE
#define ENGINE

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Engine
{
	public:
		Engine(GLuint screenWidth, GLuint screenHeight);
		~Engine();

		GLint init();
		void swapBuffer();
		GLFWwindow* getWindow() const;

	protected:
		GLFWwindow* m_window;
		GLuint m_screenWidth, m_screenHeight;
};

#endif