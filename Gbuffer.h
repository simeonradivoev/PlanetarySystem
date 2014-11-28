#pragma once
#ifndef FBO_RENDER_TEXTURE_H
#define FBO_RENDER_TEXTURE_H

#include <windows.h>
#include <gl/glew.h>
#include "renderTexture.h"
class Texture;

/**
*	A Frame Buffer Object is used by OpenGL to render into a texture. Specifically this implementation assumes that the
*	rendered model will provide diffuse, position and normal at the same time in a MRT fashion
*/
class Gbuffer
{
public:
	// Ctors/Dtors
	Gbuffer(int width, int height);
	~Gbuffer();

	// Methods
	void	start();
	void	stop();

	RenderTexture*	getDiffuseTexture() const { return m_diffuse; }
	RenderTexture*	getPositionTexture() const { return m_positions; }
	RenderTexture*	getNormalsTexture() const { return m_normals; }
	RenderTexture*	getTextureCoordsTexture() const { return m_texCoords; }
	RenderTexture*	getDepthTexture() const { return m_depth; }
	GLuint			getFrameBuffer() const { return m_fbo; }

	void BindForGeomPass(){ glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo); }
	void BindForStencilPass(){};
	void BindForReading();

private:

	// Variables
	GLuint			m_fbo; // The FBO ID
	RenderTexture*	m_diffuse;
	RenderTexture*	m_positions;
	RenderTexture*	m_normals;
	RenderTexture*	m_texCoords;
	GLuint			m_depthBuffer; // Depth buffer handle
	RenderTexture*	m_depth;

	unsigned int	m_width; // FBO width
	unsigned int	m_height; // FBO height
};
#endif //FBO_RENDER_TEXTURE_H