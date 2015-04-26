#include "texture.h"

Texture::Texture(std::string filename)
{
	m_texID = LoadGLTextures(filename.c_str());
}

Texture::~Texture()
{
}

GLuint Texture::getTexture() const
{
	return m_texID;
}

// Loads image for texture creation.
int Texture::ImageLoad(const char *filename, Image *image)
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1) 
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.

	fopen_s(&file, filename, "rb");

    // make sure the file is there.
    if (file == NULL)
    {
		printf("File Not Found : %s\n",filename);
		return 0;
    }
    
    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // read the width
    if ((i = fread(&image->sizeX, 4, 1, file)) != 1) 
	{
		printf("Error reading width from %s.\n", filename);
		return 0;
    }
    //printf("Width of %s: %lu\n", filename, image->sizeX); 
    
    // read the height 
    if ((i = fread(&image->sizeY, 4, 1, file)) != 1) 
	{
		printf("Error reading height from %s.\n", filename); 
		return 0;
    }
    //printf("Height of %s: %lu\n", filename, image->sizeY);
    
    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    if ((fread(&planes, 2, 1, file)) != 1) 
	{
		printf("Error reading planes from %s.\n", filename);
		return 0;
    }
    if (planes != 1) 
	{
		printf("Planes from %s is not 1: %u\n", filename, planes);
		return 0;
    }

    // read the bpp
    if ((i = fread(&bpp, 2, 1, file)) != 1) 
	{
		printf("Error reading bpp from %s.\n", filename);
		return 0;
    }
    if (bpp != 24) 
	{
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		return 0;
    }
	
    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data. 
    image->data = (char *) malloc(size);
    if (image->data == NULL) 
	{
		printf("Error allocating memory for color-corrected image data");
		return 0;	
    }
    if ((i = fread(image->data, size, 1, file)) != 1) 
	{
		printf("Error reading image data from %s.\n", filename);
		return 0;
    }

    for (i=0;i<size;i+=3) 
	{ 
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
    }

    return 1;
}

// Load Bitmaps And Convert To Textures
GLuint Texture::LoadGLTextures(const char* filename)
{	
    Image *image;
	GLuint textureBmp;
   
    image = (Image *) malloc(sizeof(Image));
    if (image == NULL) 
	{
		printf("Error allocating space for image");
		exit(0);
    }

    if (!ImageLoad(filename, image))
	{
		printf("Error reading image file");
		system("pause");
		exit(1);
    }

    // Create Textures	
    glGenTextures(1, &textureBmp);

    glBindTexture(GL_TEXTURE_2D, textureBmp);   // 2d texture (x and y size)
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); 
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); 
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);

	return textureBmp;
}