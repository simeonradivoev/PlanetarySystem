#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include "GameObject.h"
#include "Material.h"
#include <glm\glm.hpp>

class Shader;
class LightPass;

class Light
{
public:
	Light(glm::vec3 color,double intencity,double linearAttenuation,double expAttenuation);
	_declspec(property(get = GetColor, put = SetColor)) glm::vec3 Color;
	_declspec(property(get = GetAttenuationLinear, put = SetAttenuationLinear)) double AttenuationLinear;
	_declspec(property(get = GetAttenuationExp, put = SetAttenuationExp)) double AttenuationExp;

	double GetAttenuationExp(){ return m_attenuation_exp; }
	void SetAttenuationExp(double expAttenuation){ m_attenuation_exp = expAttenuation; }
	double GetAttenuationLinear(){ return m_attenuation_linear; }
	void SetAttenuationLinear(double linearAttenuation){ m_attenuation_linear = linearAttenuation; }
	double GetAttenuationConstant(){ return m_attenuation_constant; }
	void SetAttenuationConstant(double constantAttenuation){ m_attenuation_constant = constantAttenuation; }
	void SetColor(glm::vec3 color){ m_color = color; }
	glm::vec3 GetColor(){ return m_color; }
	double GetIntencity(){ return m_intencity; }
	double GetAmbientIntencity(){ return m_ambientIntencity; }
	void SetAmbientIntencity(double ambientIntencity){ m_ambientIntencity = ambientIntencity; }
	void GenerateBoundingSphere(Shader* shader);
	void Draw(Camera& camera, LightPass* lightPass);
	GameObject* GetBoundingSphere(){ return m_boundingSphere; }
	~Light();
protected:
	double CalcPointLightBSphere(Light& light);
	glm::vec3 m_color;
	double m_intencity;
	double m_ambientIntencity;
	double m_attenuation_linear;
	double m_attenuation_exp;
	double m_attenuation_constant;
	GameObject* m_boundingSphere;

};
#endif //LIGHT_H