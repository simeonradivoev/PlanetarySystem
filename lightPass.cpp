#include "lightPass.h"
#include "shader.h"
#include "display.h"
#include <glm\gtc\type_ptr.hpp>

Shader* LightPass::m_pointLightShader = nullptr;

LightPass::LightPass(Gbuffer* gbuffer)
{
	m_gbuffer = gbuffer;
	LightPass::m_pointLightShader = Shader::FindShader("lightPass");

	m_colorTexture_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gColorMap");
	m_positionsTexture_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPositionMap");
	m_normalsTexture_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gNormalMap");
	m_lightTexture_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gLightMap");
	m_screenSize_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gScreenSize");
	m_eyePosition = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gEyeWorldPos");

	m_lightColor_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Base.Color");
	m_lightAmbientIntencity_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Base.AmbientIntensity");
	m_lightDiffuseIntencity_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Base.DiffuseIntensity");
	m_constantAttenuation_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Atten.Constant");
	m_linearAttenuation_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Atten.Linear");
	m_expAttenuation_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Atten.Exp");
	m_position_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "gPointLight.Position");
	m_hdrExposure_u = glGetUniformLocationARB(*LightPass::m_pointLightShader, "fHdrExposure");
}

void LightPass::BindGBufferTextures(){
	//m_gbuffer->getDiffuseTexture()->Bind();
	//m_gbuffer->getPositionTexture()->Bind();
	//m_gbuffer->getNormalsTexture()->Bind();
}

void LightPass::UpdateShader(Camera& camera)
{
	glUseProgram(LightPass::m_pointLightShader->GetProgram());
	int screenWidth, screenHeight;
	glfwGetWindowSize(Display::GetCurrentDisplay()->GetWindow(), &screenWidth, &screenHeight);
	glUniform2f(m_screenSize_u, screenWidth, screenHeight);
	glUniform3fv(m_eyePosition, 1, glm::value_ptr((glm::vec3)camera.GetTransform().position));
	glUniform1f(m_hdrExposure_u, m_hdrExposure);

	glUniform1iARB(m_colorTexture_u, 0);
	glUniform1iARB(m_positionsTexture_u, 1);
	glUniform1iARB(m_normalsTexture_u, 2);
	glUniform1iARB(m_lightTexture_u, 3);
}

void LightPass::UpdateShader(Light& light)
{
	glUniform3fv(m_lightColor_u, 1, glm::value_ptr(light.GetColor()));
	glUniform1f(m_lightAmbientIntencity_u,light.GetAmbientIntencity());
	glUniform1f(m_lightDiffuseIntencity_u, light.GetIntencity());
	glUniform1f(m_constantAttenuation_u, light.GetAttenuationConstant());
	glUniform1f(m_linearAttenuation_u, light.GetAttenuationLinear());
	glUniform1f(m_expAttenuation_u, light.GetAttenuationExp());
	glUniform1f(m_expAttenuation_u, light.GetAttenuationExp());
	glUniform3fv(m_position_u, 1, glm::value_ptr((glm::vec3)(light.GetBoundingSphere()->transform.position)));
}


void LightPass::Start(Camera& camera){
	m_gbuffer->BindForLightingPass();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	
	UpdateShader(camera);
}

void LightPass::StartDrawAtmospheres(Camera& camera)
{
	

}

void LightPass::End(){
	glUseProgram(0);
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void LightPass::StencilTest()
{
	m_gbuffer->BindForStencilPass();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);

	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
}


LightPass::~LightPass()
{
	if (LightPass::m_pointLightShader != nullptr)
		delete LightPass::m_pointLightShader;
}
