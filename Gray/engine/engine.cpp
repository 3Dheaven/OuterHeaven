#include "Engine.h"

Engine::Engine(GLuint screenWidth, GLuint screenHeight):m_screenWidth(screenWidth), m_screenHeight(screenHeight)
{
}

int Engine::init()
{
	// Initialise GLFW
	if(!glfwInit())
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 8); // antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	// Open a window and create its OpenGL context 
	m_window = glfwCreateWindow( m_screenWidth, m_screenHeight, "3DHeaven", NULL, NULL); 
	if( m_window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(m_window); // Initialize GLEW 
	glewExperimental=true; // Needed in core profile 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSwapInterval(1);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return 0;
}

GLFWwindow* Engine::getWindow() const
{
	return m_window;
}

GLuint Engine::getScreenWidth() const
{
	return m_screenWidth;
}

GLuint Engine::getScreenHeight() const
{
	return m_screenHeight;
}

void Engine::swapBuffer()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

Engine::~Engine()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}