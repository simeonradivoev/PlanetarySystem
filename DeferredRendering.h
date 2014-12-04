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
	DeferredRendering(int width, int height, Gbuffer* gBuffer);
	void SetAmbientLighting(glm::vec3 color){ m_ambientLightColor = color; }
	void UpdateShader();
	// Methods
	void render(Camera& camera);
private:
	// Variables
	Shader* 		m_shader; // Deferred rendering shader
	Gbuffer*	m_gBuffer; // A pointer to the FBO render texture that contains diffuse, normals and positions

	unsigned int		m_width; // width
	unsigned int		m_height; // height

	GLuint				m_diffuseID;		// Diffuse texture handle for the shader
	GLuint				m_positionID;		// Position texture handle for the shader
	GLuint				m_normalsID;		// Normals texture handle for the shader
	GLuint				m_lightingID;		//lighting texture handle for the shader
	GLuint				m_ambientLightID;

	glm::vec3			m_ambientLightColor;
};
#endif //DEFERRED_RENDERING_H