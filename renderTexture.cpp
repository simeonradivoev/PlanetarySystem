#include "renderTexture.h"
#include <iostream>

void RenderTexture::Initialize(){
	glGenRenderbuffersEXT(1,&m_textureBuffer);
	Create(NULL);
}

void RenderTexture::RenderToTexture(Scene& scene,Camera& camera,bool isDepth){
	//allocate memory for new frame buffer
	glGenFramebuffersEXT(1, &m_frameBuffer);

	//bind the buffer and texture to the frame buffer
	BindToFrameBuffer(m_frameBuffer, isDepth ? GL_DEPTH_ATTACHMENT_EXT : GL_COLOR_ATTACHMENT0_EXT,0);
	glViewport(0, 0, getWidth(), getHeight());
	//clear the buffers
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	scene.GeometryPass(camera);
	// Stop acquiring and unbind the frame buffer
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}

void RenderTexture::BindToFrameBuffer(GLuint frameBuffer, GLenum attachmentType, unsigned int position){
	//bind the buffer just in case
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
	// Bind the render texture buffer to the frame buffer
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_textureBuffer);
	//allocate and reserve memory for buffer
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, getIternalFormat(), getWidth(), getHeight());
	//attach the buffer as a render buffer to the frame buffer
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, attachmentType + position, GL_RENDERBUFFER_EXT, m_textureBuffer);
	//atach the texture to the frame buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, attachmentType + position, GL_TEXTURE_2D, getTextureHandler(), 0);

	// Check if all worked fine
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if (status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");
}

RenderTexture::~RenderTexture()
{
	//clear any left over frame buffers
	glDeleteFramebuffersEXT(1,& m_frameBuffer);
	glDeleteRenderbuffersEXT(1, &m_textureBuffer);
}
