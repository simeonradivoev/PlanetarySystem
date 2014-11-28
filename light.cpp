#include "light.h"
#include "lightPass.h"
#include <string>


Light::Light(glm::vec3 color,double intencity, double linearAttenuation, double expAttenuation)
: m_intencity(intencity), m_attenuation_linear(linearAttenuation), m_attenuation_exp(expAttenuation), m_color(color)
{
	GenerateBoundingSphere(LightPass::GetPointLightShader());
}

void Light::GenerateBoundingSphere(Shader* shader)
{
	Material* mat = new Material(shader);
	mat->SetColor(glm::vec4(m_color.x, m_color.y, m_color.z, m_intencity));
	m_boundingSphere = GameObject::CreateSphere(CalcPointLightBSphere(*this), 10, mat);
}

double Light::CalcPointLightBSphere(Light& light){

	double MaxChannel = fmax(fmax(light.Color.x, light.Color.y), light.Color.z);

	double ret = (-light.AttenuationLinear + sqrtf(light.AttenuationLinear * light.AttenuationLinear -
		4 * light.AttenuationExp * (light.AttenuationExp - 256 * MaxChannel * light.GetIntencity())))
		/
		2 * light.AttenuationExp;
	return ret;
}

void Light::Draw(Camera& cam,LightPass* lightPass)
{
	if (m_boundingSphere != nullptr){
		lightPass->UpdateShader(*this);
		m_boundingSphere->Draw(cam, LightPass::GetPointLightShader());
	}
}


Light::~Light()
{
}
