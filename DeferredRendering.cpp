#include "DeferredRendering.h"
#include <glm\gtc\type_ptr.hpp>

/**
*	Create the deferred rendering object. I have hardcoded the shader's name here.
*/
DeferredRendering::DeferredRendering(int _dWidth, int _dHeight, FBORenderTexture* fboRenderTexture, DepthRenderTexture* shadowTexture)
: m_shader("./res/deferredRendering")
, m_fboRenderTexture(fboRenderTexture)
, m_shadowMap(shadowTexture)
	, m_width(_dWidth)
	, m_height(_dHeight)
{	
	// Get the handles from the shader
	m_diffuseID = glGetUniformLocationARB(m_shader.GetProgram(),"tDiffuse");
	m_positionID = glGetUniformLocationARB(m_shader.GetProgram(), "tPosition");
	m_normalsID = glGetUniformLocationARB(m_shader.GetProgram(), "tNormals");
	m_shadowMapID = glGetUniformLocationARB(m_shader.GetProgram(), "tShadowMap");
	m_cameraPositionID = glGetUniformLocationARB(m_shader.GetProgram(),"cameraPosition");

	m_worldToCameraViewMatrixID = glGetUniformLocationARB(m_shader.GetProgram(),"worldToCameraViewMatrix");
	m_lightViewToProjectionMatrixID = glGetUniformLocationARB(m_shader.GetProgram(), "lightViewToProjectionMatrix");
	m_worldToLightViewMatrixID = glGetUniformLocationARB(m_shader.GetProgram(), "worldToLightViewMatrix");
}

/**
*	Render the big quad with the deferred rendering shader on it
*/
void DeferredRendering::render(Camera& camera)
{
	//Projection setup
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,m_width,0,m_height,0.1f,2);	
	
	//Model setup
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glUseProgramObjectARB(m_shader.GetProgram());

	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getDiffuseTexture());
	glUniform1iARB ( m_diffuseID, 0 );
	
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getPositionTexture());
	glUniform1iARB ( m_positionID, 1 );
	
	glActiveTextureARB(GL_TEXTURE2_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getNormalsTexture());
	glUniform1iARB ( m_normalsID, 2 );

	glActiveTextureARB(GL_TEXTURE3_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap->getTexture());
	glUniform1iARB(m_shadowMapID, 3);

	glUniformMatrix4fv(m_worldToCameraViewMatrixID, 1, GL_FALSE, glm::value_ptr((glm::mat4)m_worldToCameraViewMatrix));
	glUniformMatrix4fv(m_lightViewToProjectionMatrixID, 1, GL_FALSE, glm::value_ptr((glm::mat4)m_lightViewToProjectionMatrix));
	glUniformMatrix4fv(m_worldToLightViewMatrixID, 1, GL_FALSE, glm::value_ptr((glm::mat4)m_worldToLightViewMatrix));
	glUniform3fv(m_cameraPositionID, 1, glm::value_ptr((glm::vec3)camera.transform.position));

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

void DeferredRendering::startRenderToFBO(){
	m_fboRenderTexture->start();
}

void DeferredRendering::endRenderToFBO(){
	m_fboRenderTexture->stop();
}

void DeferredRendering::startRenderToShadowMap(){
	m_shadowMap->start();
}

void DeferredRendering::endRenderToShadowMap(){
	m_shadowMap->stop();
}

void DeferredRendering::render(unsigned int target)
{
	//Projection setup
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_width, 0, m_height, 0.1f, 2);

	//Model setup
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glUseProgramObjectARB(m_shader.GetProgram());

	if (target == 0){
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getDiffuseTexture());
		glUniform1iARB(m_diffuseID, 0);
	}
	else if (target == 1)
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getPositionTexture());
		glUniform1iARB(m_positionID, 0);
	}
	else
	{
		glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fboRenderTexture->getNormalsTexture());
		glUniform1iARB(m_normalsID, 0);
	}

	// Render the quad
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, -1.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(1, 0);
	glVertex3f((float)m_width, 0.0f, 0.0f);
	glTexCoord2f(1, 1);
	glVertex3f((float)m_width, (float)m_height, 0.0f);
	glTexCoord2f(0, 1);
	glVertex3f(0.0f, (float)m_height, 0.0f);
	glEnd();

	// Reset OpenGL state
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgramObjectARB(0);

	//Reset to the matrices	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}