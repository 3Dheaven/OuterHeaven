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
#include "model/Model.h"
#include "shader/Shader.h"
#include "light/PointLight/PointLight.h"

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

		Model ground("../obj/scene/scene.obj");
		Shader shaders("shaders/vertex.glsl", "shaders/fragment.glsl");

		Model light("../obj/sphereLight.obj");
		Shader shaderLight("shaders/vertexLight.glsl", "shaders/fragmentLight.glsl");

		PointLight pLight;

		// Point light positions
		glm::vec3 pointLightPositions[] = {
			glm::vec3(0.0f, 4.0f, -7.0f),
			glm::vec3(-40.0f, 4.0f, -7.0f)
		};

		if(glfwJoystickPresent(0))
		{
			cout << glfwGetJoystickName(0) << " : detected" << endl;
			cout << "Left stick : movement control" << endl;
			cout << "Right stick : camera control" << endl;
		}

		double ellapsed_time = glfwGetTime();

		while(!glfwWindowShouldClose(window))
		{
			// Set frame time
			GLdouble currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Check and call events
			glfwPollEvents();
			Do_Movement();

			glClearColor(0.03f, 0.03f, 0.03f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			shaders.Use();

			// Transformation matrices
			glm::mat4 projection = glm::perspective(camera.getZoom(), (float)screenWidth/(float)screenHeight, 0.1f, 500.0f);
			glm::mat4 view = camera.GetViewMatrix();
			glUniformMatrix4fv(glGetUniformLocation(shaders.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(shaders.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			// Point light 1
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);		
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);		
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[0].diffuse"), 1.0f, 1.0f, 1.0f); 
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[0].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[0].linear"), 0.009f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[0].quadratic"), 0.0032f);

			// Point light 2
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);		
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);		
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[1].diffuse"), 1.0f, 1.0f, 1.0f); 
			glUniform3f(glGetUniformLocation(shaders.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[1].constant"), 1.0f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[1].linear"), 0.009f);
			glUniform1f(glGetUniformLocation(shaders.Program, "pointLights[1].quadratic"), 0.0032f);

			ground.Draw(shaders);

			shaderLight.Use();

			// Transformation matrices
			glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

			// Draw the loaded model
			glm::mat4 modelLightMatrix = glm::translate(glm::mat4(1.0f), pointLightPositions[0]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLightMatrix));

			light.Draw(shaderLight);

			modelLightMatrix = glm::translate(glm::mat4(1.0f), pointLightPositions[1]) * glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(glGetUniformLocation(shaderLight.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLightMatrix));

			light.Draw(shaderLight);

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
		camera.ProcessJoystickPad(tab[0], tab[1], tab[3], tab[4]);
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