#include "Gbuffer.h"
#include <exception>
#include "renderTexture.h"
#include "texture.h"

/**
*	Create the FBO render texture initializing all the stuff that we need
*/
Gbuffer::Gbuffer(int _dWidth, int _dHeight)
{	
	// Save extensions
	m_width  = _dWidth;
	m_height = _dHeight;

	// Generate the OGL resources for what we need
	glGenFramebuffersEXT(1, &m_fbo);
	m_diffuse = new RenderTexture(m_width, m_height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true,GL_NEAREST);
	m_positions = new RenderTexture(m_width, m_height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_normals = new RenderTexture(m_width, m_height, GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_texCoords = new RenderTexture(m_width, m_height, GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_depth = new RenderTexture(m_width, m_height, GL_DEPTH_COMPONENT24, GL_DEPTH_COMPONENT, GL_FLOAT, true, GL_NEAREST);

	// Bind the FBO so that the next operations will be bound to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	m_diffuse->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT,0);
	m_positions->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 1);
	m_normals->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 2);
	m_texCoords->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 3);
	m_depth->BindToFrameBuffer(m_fbo,GL_DEPTH_ATTACHMENT_EXT,0);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT };
	glDrawBuffers(4, buffers);

	// Check if all worked fine and unbind the FBO
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Gbuffer::BindForReading()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glEnable(GL_TEXTURE_2D);

	m_diffuse->Bind(0);
	m_positions->Bind(1);
	m_normals->Bind(2);
	m_texCoords->Bind(3);
}

/**
*	Destructor
*/
Gbuffer::~Gbuffer(){
	delete m_diffuse;
	delete m_normals;
	delete m_positions;
	delete m_depth;
}

/**
*	Start rendering to the texture
*	Both color and depth buffers are cleared.
*/
void Gbuffer::start(){

	BindForGeomPass();
	
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0,0,m_width, m_height);

	// Clear the render targets
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);

	glActiveTextureARB(GL_TEXTURE0_ARB);
	
}

/**
*	Stop rendering to this texture.
*/
void Gbuffer::stop(){	

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	// Stop acquiring and unbind the FBO
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	glPopAttrib();
}