#include "texture2D.h"
#include "stb_image.h"

Texture2D::Texture2D(const std::string& fileName, bool clamp)
{
	int width, heigh, numComponents;
	stbi_uc* imageData = stbi_load(fileName.c_str(), &width, &heigh, &numComponents, 4);

	if (imageData != NULL)
	{
		Initialize(width, heigh, GL_RGB, GL_RGBA,GL_UNSIGNED_BYTE, clamp);
		Create(imageData,true);
	}
	else
	{
		std::cerr << "Texture loading failed for texture" << fileName << std::endl;
	}

	stbi_image_free(imageData);
}

Texture2D::~Texture2D()
{
}
