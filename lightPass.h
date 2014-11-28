#pragma once
#ifndef LIGHT_PASS_H
#define LIGHT_PASS_H

#include "Gbuffer.h"
#include "light.h"
#include "camera.h"

class Shader;

class LightPass
{
public:
	LightPass(Gbuffer* gbuffer,Shader* pointLighShader);
	~LightPass();
	static Shader* GetPointLightShader(){ return m_pointLightShader; }
	void Start(Camera& camera);
	void End();
	void UpdateShader(Camera& camera);
	void UpdateShader(Light& light);
	void BindGBufferTextures();
private:
	Gbuffer* m_gbuffer;
	static Shader* m_pointLightShader;

	GLuint m_colorTexture_u;
	GLuint m_positionsTexture_u;
	GLuint m_normalsTexture_u;
	GLuint m_texCoordsTexture_u;
	GLuint m_screenSize_u;
	GLuint m_eyePosition;

	GLuint m_lightColor_u;
	GLuint m_lightAmbientIntencity_u;
	GLuint m_lightDiffuseIntencity_u;
	GLuint m_linearAttenuation_u;
	GLuint m_constantAttenuation_u;
	GLuint m_expAttenuation_u;
	GLuint m_position_u;

	void DrawAmbientLight();
	void UpdateShader(glm::vec3 color, float intencity);
};
#endif //LIGHT_PASS_H