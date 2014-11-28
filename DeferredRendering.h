#pragma once
#ifndef DEFERRED_RENDERING_H
#define DEFERRED_RENDERING_H

#include "shader.h"
#include "camera.h"
#include "Scene.h"
#include "DepthRenderTexture.h"
#include <glm\glm.hpp>
#include <gl/glew.h>
class Gbuffer;
class RenderTexture;


/**
*	This object is used to render a big screen sized quad with the deferred rendering shader applied on it.
*/
class DeferredRendering
{
public:
	// Ctors/Dtors
	DeferredRendering(int width, int height, Gbuffer* fboRenderTexture);

	// Methods
	void render(Camera& camera);
	void render(unsigned int target);
	void startRenderToFBO();
	void endRenderToFBO();
	void renderToShadowMap(Scene& scene,Camera& camera);
	void setLightMatix(glm::dmat4 worldToCameraViewMatrix, glm::dmat4 lightViewToProjectionMatrix, glm::dmat4 worldToLightViewMatrix)
	{
		m_worldToCameraViewMatrix = worldToCameraViewMatrix;
		m_lightViewToProjectionMatrix = lightViewToProjectionMatrix;
		m_worldToLightViewMatrix = worldToLightViewMatrix;
	}

private:
	// Variables
	Shader 		m_shader; // Deferred rendering shader
	Gbuffer*	m_fboRenderTexture; // A pointer to the FBO render texture that contains diffuse, normals and positions

	unsigned int		m_width; // width
	unsigned int		m_height; // height

	GLuint				m_diffuseID;		// Diffuse texture handle for the shader
	GLuint				m_positionID;		// Position texture handle for the shader
	GLuint				m_normalsID;		// Normals texture handle for the shader
	GLuint				m_texCoordsID;		//textureCoordinates texture handle for the shader

	GLuint m_cameraPositionID;	//camera position handle for the shader
	GLuint m_worldToCameraViewMatrixID;
	GLuint m_lightViewToProjectionMatrixID;
	GLuint m_worldToLightViewMatrixID;

	glm::dmat4 m_worldToCameraViewMatrix;
	glm::dmat4 m_lightViewToProjectionMatrix;
	glm::dmat4 m_worldToLightViewMatrix;
};
#endif //DEFERRED_RENDERING_H