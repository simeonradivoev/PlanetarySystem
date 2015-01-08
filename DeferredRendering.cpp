#include "DeferredRendering.h"
#include "renderTexture.h"
#include "Gbuffer.h"
#include <glm\gtc\type_ptr.hpp>
#include "Core\Input.h"

/**
*	Create the deferred rendering object. I have hardcoded the shader's name here.
*/
DeferredRendering::DeferredRendering(int _dWidth, int _dHeight, Gbuffer* gBuffer)
: m_shader(Shader::FindShader("deferredRendering"))
, m_gBuffer(gBuffer)
	, m_width(_dWidth)
	, m_height(_dHeight)
{	
	// Get the handles from the shader

	m_positionID = glGetUniformLocationARB(m_shader->GetProgram(), "tPosition");
	m_diffuseID = glGetUniformLocationARB(m_shader->GetProgram(), "tDiffuse");
	m_normalsID = glGetUniformLocationARB(m_shader->GetProgram(), "tNormals");
	m_lightingID = glGetUniformLocationARB(m_shader->GetProgram(), "tLighting");
	m_ambientLightID = glGetUniformLocationARB(m_shader->GetProgram(), "ambientLight");
}

void DeferredRendering::UpdateShader()
{
	if (Input::MouseVisible)
	{
		glUniform3fvARB(m_ambientLightID,1,glm::value_ptr(Display::GetCurrentDisplay()->GetClearColor()));
	}
	else
	{
		glUniform3fARB(m_ambientLightID, 0, 0, 0);
	}

	glUniform1fARB(glGetUniformLocationARB(m_shader->GetProgram(), "fWidth"), m_width);
	glUniform1fARB(glGetUniformLocationARB(m_shader->GetProgram(), "fHeight"), m_height);
	glUniform1fARB(glGetUniformLocationARB(m_shader->GetProgram(), "fHdrExposure"), 0.05f);
}

/**
*	Render the big quad with the deferred rendering shader on it
*/
void DeferredRendering::render(Camera& camera)
{
	glUseProgramObjectARB(m_shader->GetProgram());
	UpdateShader();

	//Projection setup
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_width,0,m_height,0.1f,2);	
	
	//Model setup
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	
	glUniform1iARB(m_diffuseID, 0);
	glUniform1iARB(m_positionID, 1);
	glUniform1iARB(m_normalsID, 2);
	glUniform1iARB(m_lightingID, 3);

	// Render the quad
	glLoadIdentity();
	glColor3f(1,1,1);
	glTranslatef(0,0,-1.0);
	
	glBegin(GL_QUADS);
	glTexCoord2f( 0, 0 );
	glVertex3f(    0.0f, 0.0f, 0.0f);
	glTexCoord2f( 1, 0 );
	glVertex3f(   (float) m_width, 0.0f, 0.0f);
	glTexCoord2f( 1, 1 );
	glVertex3f(   (float) m_width, (float) m_height, 0.0f);
	glTexCoord2f( 0, 1 );
	glVertex3f(    0.0f,  (float) m_height, 0.0f);
	glEnd();
	
	// Reset OpenGL state
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE2_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	glUseProgramObjectARB(0);
	
	//Reset to the matrices	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}