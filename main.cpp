#pragma comment(lib, "GLFW3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp.lib")

#include <windows.h>
#include "camera\camera.h"
#include "engine\engine.h"
#include "molecule\molecule.h"

/* sadly we put it here, so that it can be referenced in the callbacks */
static TrackBallCamera trackBallCamera;

void errorCallback( int error, const char * desc );
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod);
void frameBufferSizeCallback( GLFWwindow *window, int width, int height );
void scrollCallback( GLFWwindow *window, double x, double y );
void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods );
void cursorCallback( GLFWwindow *window, double x, double y );

void errorCallback( int error, const char * desc )
{
    fputs( desc, stderr );
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mod) {
    if( action == GLFW_PRESS ) {
        switch ( key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose( window, GL_TRUE );
                return;
            default:
                break;
        }
    }
}

void frameBufferSizeCallback( GLFWwindow *window, int width, int height )
{
    glViewport( 0, 0, width, height );
}

void scrollCallback( GLFWwindow *window, double x, double y )
{
	trackBallCamera.OnScrolling( window, x, y );
}

void mouseButtonCallback( GLFWwindow * window, int button, int action, int mods )
{
    trackBallCamera.OnMouseButton( window, button, action, mods );
}

void cursorCallback( GLFWwindow *window, double x, double y )
{
    trackBallCamera.OnMouseMotion( window, x, y );
}

GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
 
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
 
    GLint Result = GL_FALSE;
    int InfoLogLength;
 
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_file_path);
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> VertexShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_file_path);
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
 
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
    glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
 
    // Link the program
    fprintf(stdout, "Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);
 
    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( glm::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
 
    return ProgramID;
}

int main()
{
	Engine engine;
	engine.init();
	GLFWwindow* window = engine.getWindow();

    glfwSetErrorCallback( errorCallback );
    glfwSetScrollCallback( window, scrollCallback );
    glfwSetKeyCallback( window, keyCallback );
    glfwSetFramebufferSizeCallback( window, frameBufferSizeCallback );
    glfwSetCursorPosCallback( window, cursorCallback );
    glfwSetMouseButtonCallback( window, mouseButtonCallback );

	double ellapsed_time = glfwGetTime();

	GLuint programID = LoadShaders( "shaders/vertex.glsl", "shaders/fragment.glsl" );

	Molecule molecule(programID, &trackBallCamera, window);

	glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

	do
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		molecule.display();

		engine.swapBuffer();

        if ((glfwGetTime() - ellapsed_time)/1000.0 < 16.0)
			Sleep((DWORD)(16.0 - (glfwGetTime() - ellapsed_time)/1000.0));
		ellapsed_time = glfwGetTime();

	}
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0 );

	return 0;
}