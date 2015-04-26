#ifndef OBJECT
#define OBJECT

#include <iostream>
#include <string>
#include <vector>

#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Object
{
	public:
		Object(std::string filename);
		bool loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, 
					std::vector < glm::vec2 > & out_uvs, std::vector < glm::vec3 > & out_normals);
		void bufferData();
		void display();
		~Object();

	protected:
		std::vector< glm::vec3 > m_vertices;
		std::vector< glm::vec2 > m_uvs;
		std::vector< glm::vec3 > m_normals;
		
		GLuint m_vertexArrayID;
		GLuint m_vertexbuffer;
		GLuint m_uvbuffer;
		GLuint m_normalbuffer;
};

#endif