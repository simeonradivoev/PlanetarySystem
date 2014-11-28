#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL\glew.h>

class Texture
{
public:
	Texture(){}
	Texture(unsigned int width,unsigned int height);
	Texture(unsigned int width, unsigned int height, GLenum iternalFormat){ Initialize(width, height, iternalFormat); }
	Texture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format){ Initialize(width, height, iternalFormat,format); }
	Texture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type){ Initialize(width, height, iternalFormat, format, type); }
	Texture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type, bool clamp){ Initialize(width, height, iternalFormat, format, type, clamp); }
	Texture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type, bool clamp, GLenum filter){ Initialize(width, height, iternalFormat, format, type, clamp, filter); }
	void CreateEmpty();
	void Bind(unsigned int uint);
	void Bind();
	void Unbind();
	void Unbind(unsigned int unit);
	virtual ~Texture();
	unsigned int getHeight(){ return m_height; }
	unsigned int getWidth(){ return m_width; }
	void setWrap(GLint type){ Bind(); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type); }
	void setFiltering(GLfloat type){ Bind(); glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type); glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type); }
	GLenum getFormat(){ return m_format; }
	GLenum getIternalFormat(){ return m_iternalFormat; }
	GLenum getType(){ return m_type; }
	GLuint& getTextureHandler(){ return m_texture; }
protected:
	void Initialize(unsigned int width, unsigned int height, GLenum iternalFormat = GL_RGB, GLenum format = GL_RGBA,GLenum type = GL_UNSIGNED_BYTE, bool clamp = false,GLenum filter = GL_LINEAR);
	void Create(const void *pixels);
private:
	GLuint m_texture;
	unsigned int m_height;
	unsigned int m_width;
	bool m_clamp;
	GLenum m_iternalFormat;
	GLenum m_format;
	GLenum m_type;
	GLenum m_filter;
};
#endif TEXTURE_H