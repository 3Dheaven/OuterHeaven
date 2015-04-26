#ifndef TEXTURE
#define TEXTURE

#include <iostream>
#include <string>
#include <vector>

#include <GLFW/glfw3.h>

struct Image
{
	int sizeX, sizeY;
	char* data;
};

class Texture
{
	public:
		Texture(std::string filename);
		GLuint LoadGLTextures(const char* filename);
		int ImageLoad(const char *filename, Image *image);
		GLuint getTexture() const;
		~Texture();

	protected:
		GLuint m_texID;
};

#endif