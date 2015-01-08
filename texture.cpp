#include "texture.h"
#include "stb_image.h"
#include <cassert>

void Texture::Initialize(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format,GLenum type, bool clamp,GLenum filter)
{
	m_width = width;
	m_height = height;
	m_iternalFormat = iternalFormat;
	m_format = format;
	m_type = type;
	m_clamp = clamp;
	m_filter = filter;
}

void Texture::Create(const void *pixels,bool mipmaps){
	//Alocate space for texture in GPU and return handler
	glGenTextures(1, &m_texture);
	//bind the texture for use
	Bind();
	//Texture wraping
	GLint clamp = m_clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT;
	setWrap(clamp);
	//Texture filtering
	setFiltering(mipmaps ? GL_NEAREST_MIPMAP_LINEAR : m_filter);

	if (mipmaps)
		glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP,GL_TRUE);

	glTexImage2D(GL_TEXTURE_2D, 0, m_iternalFormat, m_width, m_height, 0, m_format, m_type, pixels);
	
}

void Texture::CreateEmpty(){
	Create(NULL);
}

Texture::~Texture()
{
	glDeleteTextures(1,&m_texture);
}

void Texture::Bind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	Bind();
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unbind(unsigned int unit)
{
	assert(unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, 0);
}