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
	m_diffuse = new RenderTexture(m_width, m_height, GL_RGBA, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_positions = new RenderTexture(m_width, m_height, GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_normals = new RenderTexture(m_width, m_height, GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT, true, GL_NEAREST);
	m_lighting = new RenderTexture(m_width, m_height, GL_RGBA32F_ARB, GL_RGB, GL_FLOAT, true, GL_NEAREST);
	m_depthTexture = new RenderTexture(m_width, m_height, GL_DEPTH32F_STENCIL8, GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, true, GL_NEAREST);


	// Bind the FBO so that the next operations will be bound to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fbo);

	m_diffuse->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT,0);
	m_positions->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 1);
	m_normals->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 2);
	m_lighting->BindToFrameBuffer(m_fbo, GL_COLOR_ATTACHMENT0_EXT, 3);
	m_depthTexture->BindToFrameBuffer(m_fbo,GL_DEPTH_STENCIL_ATTACHMENT, 0);

	// Check if all worked fine and unbind the FBO
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if( status != GL_FRAMEBUFFER_COMPLETE_EXT)
		throw new std::exception("Can't initialize an FBO render texture. FBO initialization failed.");

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void Gbuffer::BindForGeomPass()
{

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	GLenum buffers[] = { GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT };
	glDrawBuffers(3, buffers);
}

void Gbuffer::BindForStencilPass()
{
	glDrawBuffer(GL_NONE);
}

void Gbuffer::BindForLightingPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	//output buffer only on position 0
	glDrawBuffer(GL_COLOR_ATTACHMENT3_EXT);

	m_diffuse->Bind(0);
	m_positions->Bind(1);
	m_normals->Bind(2);
}

void Gbuffer::BindForTransperatPass()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
	Display::GetCurrentDisplay()->Clear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_BLEND);
}

void Gbuffer::BindForFinalPass()
{
	m_diffuse->Bind(0);
	m_positions->Bind(1);
	m_normals->Bind(2);
	m_lighting->Bind(3);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	//glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, Display::GetCurrentDisplay()->GetWidth(), Display::GetCurrentDisplay()->GetHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void Gbuffer::StartFrame()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT4_EXT);
	glClear(GL_COLOR_BUFFER_BIT);
}

/**
*	Destructor
*/
Gbuffer::~Gbuffer(){
	delete m_diffuse;
	delete m_normals;
	delete m_positions;
}

/**
*	Start rendering to the texture
*	Both color and depth buffers are cleared.
*/
void Gbuffer::start(){

	BindForGeomPass();
	
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	// Clear the render targets
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable(GL_DEPTH_TEST);
	
}

/**
*	Stop rendering to this texture.
*/
void Gbuffer::stop(){
	
}