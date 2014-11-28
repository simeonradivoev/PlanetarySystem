#pragma once
#ifndef RENDER_TEXTURE_H
#define RENDER_TEXTURE_H

#include "texture.h"
#include "Scene.h"
#include "camera.h"
#include <GL\glew.h>

class RenderTexture : public Texture
{
public:
	RenderTexture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type) :Texture(width, height, iternalFormat, format, type){ Initialize(); }
	RenderTexture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type, bool clamp) :Texture(width, height, iternalFormat, format, type, clamp){ Initialize(); }
	RenderTexture(unsigned int width, unsigned int height, GLenum iternalFormat, GLenum format, GLenum type, bool clamp, GLenum filter) :Texture(width, height, iternalFormat, format, type, clamp, filter){ Initialize(); }
	void RenderToTexture(Scene& scene,Camera& camera, bool isDepth = false);
	void BindToFrameBuffer(GLuint frameBuffer, GLenum attachmentType,unsigned int position);
	GLuint GetTextureBuffer(){ return m_textureBuffer; }
	~RenderTexture();
private:
	void Initialize();
	void StartRender();
	void EndRender();
	GLuint m_frameBuffer;
	GLuint m_textureBuffer;
};
#endif //RENDER_TEXTURE_H