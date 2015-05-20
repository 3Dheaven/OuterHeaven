#pragma comment(lib, "GLFW3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "SOIL.lib")

#include <windows.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera/camera.h"
#include "engine/engine.h"
#include "model/nature/Nature.h"
#include "shader/Shader.h"

#include "framebuffer\framebuffer.h"

Camera camera;
bool keys[1024];
GLdouble lastX = 400, lastY = 300;
bool firstMouse = true;

GLdouble deltaTime = 0.0f, lastFrame = 0.0f;

// Properties
GLuint screenWidth = 1280, screenHeight = 720;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

int main()
{
	Engine engine(screenWidth, screenHeight);
	if(!engine.init())
	{
		GLFWwindow* window = engine.getWindow();

		// Set the required callback functions
		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		Model crytekSponza("../obj/crytek-sponza/sponza.obj", camera, engine);
		//Model teapot("../obj/teapot/teapot.obj", camera, engine);
		//Model dabrovicSponza("../obj/dabrovic-sponza/sponza.obj", camera, engine);
		//Model sibenikCathedral("../obj/sibenik-cathedral/sibenik.obj", camera, engine);
		//Model nanosuit("../obj/nanosuit/nanosuit.obj", camera, engine);
		//Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");

		if(glfwJoystickPresent(0))
		{
			cout << glfwGetJoystickName(0) << " : detected" << endl;
			cout << "Left stick : movement control" << endl;
			cout << "Right stick : camera control" << endl;
		}

		double ellapsed_time = glfwGetTime();

		Framebuffer::vertexQuad();
		Framebuffer frameBuffer(true, screenWidth, screenHeight);

		while(!glfwWindowShouldClose(window))
		{
			// Set frame time
			GLdouble currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Check and call events
			glfwPollEvents();
			Do_Movement();

			glClearDepth(1.0);
			glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//frameBuffer.enable();
			//glViewport(0, 0, 1280, 720);
			crytekSponza.Draw();
			//frameBuffer.disable();

			/*
			//glViewport(0, 0, 1280, 720);

			glClearDepth(1.0);
			glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shader.Use();

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, frameBuffer.getRenderedTexture());

			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, frameBuffer.getDepthTexture());
			
			Framebuffer::drawQuad();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			*/

			/*
			glEnable(GL_SCISSOR_TEST);
			glScissor(0, 0, 1280, 720);
			glClear(GL_DEPTH_BUFFER_BIT);
			glViewport(0, 0, 300, 168);

			glEnable(GL_TEXTURE_2D);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, frameBuffer.getRenderedTexture());
			glUniform1i(glGetUniformLocation(shader.Program, "texBuffer"), 0);
			
			Framebuffer::drawQuad();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisable(GL_TEXTURE_2D);
			*/
			
			engine.swapBuffer();

			/*
			if((glfwGetTime() - ellapsed_time) / 1000.0 < 16.67)
				Sleep(static_cast<DWORD>(16.67 - ((glfwGetTime() - ellapsed_time) / 1000.0)));
			ellapsed_time = glfwGetTime();
			*/
		}
	}

	return 0;
}

// Moves/alters the camera positions based on user input
void Do_Movement()
{
    // Camera controls
    if(keys[GLFW_KEY_Z])
        camera.ProcessKeyboard(FORWARD, (GLfloat) deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, (GLfloat) deltaTime);
    if(keys[GLFW_KEY_Q])
        camera.ProcessKeyboard(LEFT, (GLfloat) deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, (GLfloat) deltaTime);

	if(glfwJoystickPresent(0))
	{
		int nb;
		const float* tab = glfwGetJoystickAxes(0, &nb);
		camera.ProcessJoystickPad(tab[0], tab[1], tab[3], tab[4], (GLfloat) deltaTime);
	}
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLdouble xoffset = xpos - lastX;
    GLdouble yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement((GLfloat) xoffset, (GLfloat) yoffset);
}	

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll((GLfloat) yoffset);
}