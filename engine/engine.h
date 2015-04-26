#ifndef ENGINE
#define ENGINE

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

class Engine
{
	public:
		Engine();
		~Engine();

		GLint init();
		void swapBuffer();
		GLFWwindow* getWindow() const;

	protected:
		GLFWwindow* m_window;
};

#endif