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
	void StencilTest();
private:
	Gbuffer* m_gbuffer;
	static Shader* m_pointLightShader;

	GLuint m_colorTexture_u;
	GLuint m_positionsTexture_u;
	GLuint m_normalsTexture_u;
	GLuint m_lightTexture_u;
	GLuint m_screenSize_u;
	GLuint m_eyePosition;

	GLuint m_lightColor_u;
	GLuint m_lightAmbientIntencity_u;
	GLuint m_lightDiffuseIntencity_u;
	GLuint m_linearAttenuation_u;
	GLuint m_constantAttenuation_u;
	GLuint m_expAttenuation_u;
	GLuint m_position_u;
};
#endif //LIGHT_PASS_H