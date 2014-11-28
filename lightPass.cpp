#include "lightPass.h"
#include "shader.h"
#include "display.h"
#include <glm\gtc\type_ptr.hpp>

Shader* LightPass::m_pointLightShader = nullptr;

LightPass::LightPass(Gbuffer* gbuffer, Shader* pointLighShader)
{
	m_gbuffer = gbuffer;
	LightPass::m_pointLightShader = pointLighShader;

	m_colorTexture_u = glGetUniformLocationARB(*pointLighShader, "gColorMap");
	m_positionsTexture_u = glGetUniformLocationARB(*pointLighShader, "gPositionMap");
	m_normalsTexture_u = glGetUniformLocationARB(*pointLighShader, "gNormalMap");
	m_screenSize_u = glGetUniformLocationARB(*pointLighShader, "gScreenSize");
	m_eyePosition = glGetUniformLocationARB(*pointLighShader, "gEyeWorldPos");

	m_lightColor_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Base.Color");
	m_lightAmbientIntencity_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Base.AmbientIntensity");
	m_lightDiffuseIntencity_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Base.DiffuseIntensity");
	m_constantAttenuation_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Atten.Constant");
	m_linearAttenuation_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Atten.Linear");
	m_expAttenuation_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Atten.Exp");
	m_position_u = glGetUniformLocationARB(*pointLighShader, "gPointLight.Position");

}

void LightPass::BindGBufferTextures(){
	//m_gbuffer->getDiffuseTexture()->Bind();
	//m_gbuffer->getPositionTexture()->Bind();
	//m_gbuffer->getNormalsTexture()->Bind();
}

void LightPass::UpdateShader(Camera& camera){
	int screenWidth, screenHeight;
	glfwGetWindowSize(Display::GetCurrentDisplay()->GetWindow(), &screenWidth, &screenHeight);
	glUniform2f(m_screenSize_u, screenWidth, screenHeight);
	glUniform3fv(m_eyePosition, 1, glm::value_ptr((glm::vec3)camera.GetTransform().position));

	glUniform1iARB(m_colorTexture_u, 0);
	glUniform1iARB(m_positionsTexture_u, 1);
	glUniform1iARB(m_normalsTexture_u, 2);
}

void LightPass::UpdateShader(Light& light)
{
	glUniform3fv(m_lightColor_u, 1, glm::value_ptr(light.GetColor()));
	glUniform1f(m_lightAmbientIntencity_u,light.GetAmbientIntencity());
	glUniform1f(m_lightDiffuseIntencity_u, light.GetIntencity());
	glUniform1f(m_constantAttenuation_u, light.GetAttenuationConstant());
	glUniform1f(m_linearAttenuation_u, light.GetAttenuationLinear());
	glUniform1f(m_expAttenuation_u, light.GetAttenuationExp());
	glUniform3fv(m_position_u,1, glm::value_ptr((glm::vec3)light.GetBoundingSphere()->transform.position));
}

void LightPass::UpdateShader(glm::vec3 color,float intencity)
{
	glUniform3fv(m_lightColor_u, 1, glm::value_ptr(color));
	glUniform1f(m_lightAmbientIntencity_u, intencity);
	glUniform1f(m_lightDiffuseIntencity_u, 0);
	glUniform1f(m_constantAttenuation_u, 0);
	glUniform1f(m_linearAttenuation_u, 0);
	glUniform1f(m_expAttenuation_u, 0);
	glUniform3f(m_position_u, 0, 0,0);
	LightPass::m_pointLightShader->Reset();
}

void LightPass::DrawAmbientLight(){
	int m_width, m_height;
	glfwGetWindowSize(Display::GetCurrentDisplay()->GetWindow(),&m_width,&m_height);

	//Projection setup
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, m_width, 0, m_height, 0.1f, 2);

	//Model setup
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Render the quad
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glTranslatef(0, 0, -1.0);

	glBegin(GL_QUADS);
	glVertex3f(-1, -1, -1); 
	glVertex3f(1, -1, -1);
	glVertex3f(1, 1, -1);
	glVertex3f(-1, 1, -1);
	glEnd();

	//Reset to the matrices	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void LightPass::Start(Camera& camera){
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glUseProgram(LightPass::m_pointLightShader->GetProgram());
	UpdateShader(camera);
	//UpdateShader(glm::vec3(1,1,1),0.1);
	//DrawAmbientLight();
}

void LightPass::End(){
	glUseProgram(0);
}


LightPass::~LightPass()
{
	if (LightPass::m_pointLightShader != nullptr)
		delete LightPass::m_pointLightShader;
}
