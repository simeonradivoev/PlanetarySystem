#pragma once
#include <gl/glew.h>
#include "shader.h"
#include "camera.h"
#include "FBORenderTexture.h"
#include "DepthRenderTexture.h"
#include "glm\glm.hpp"

/**
*	This object is used to render a big screen sized quad with the deferred rendering shader applied on it.
*/
class DeferredRendering
{
public:
	// Ctors/Dtors
	DeferredRendering(int width, int height, FBORenderTexture* fboRenderTexture, DepthRenderTexture* shadowTexture);

	// Methods
	void render(Camera& camera);
	void render(unsigned int target);
	void startRenderToFBO();
	void endRenderToFBO();
	void startRenderToShadowMap();
	void endRenderToShadowMap();
	void setLightMatix(glm::dmat4 worldToCameraViewMatrix, glm::dmat4 lightViewToProjectionMatrix, glm::dmat4 worldToLightViewMatrix)
	{
		m_worldToCameraViewMatrix = worldToCameraViewMatrix;
		m_lightViewToProjectionMatrix = lightViewToProjectionMatrix;
		m_worldToLightViewMatrix = worldToLightViewMatrix;
	}

private:
	// Variables
	Shader 		m_shader; // Deferred rendering shader
	FBORenderTexture*	m_fboRenderTexture; // A pointer to the FBO render texture that contains diffuse, normals and positions
	DepthRenderTexture* m_shadowMap;

	unsigned int		m_width; // width
	unsigned int		m_height; // height

	GLuint				m_diffuseID; // Diffuse texture handle for the shader
	GLuint				m_positionID; // Position texture handle for the shader
	GLuint				m_normalsID; // Normals texture handle for the shader
	GLuint				m_shadowMapID;
	GLuint				m_cameraPositionID;

	GLuint m_worldToCameraViewMatrixID;
	GLuint m_lightViewToProjectionMatrixID;
	GLuint m_worldToLightViewMatrixID;

	glm::dmat4 m_worldToCameraViewMatrix;
	glm::dmat4 m_lightViewToProjectionMatrix;
	glm::dmat4 m_worldToLightViewMatrix;
};
