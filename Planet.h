#pragma once
#ifndef PLANET_H
#define PLANET_H

#include "camera.h"
#include "light.h"
#include <string>
#include <list>
#include <glm\glm.hpp>

class GameObject;
class Material;
class Shader;
class Texture;
class LightPass;

class Planet
{
public:
	Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius,double m_lightIntencity,Shader* shader,Texture* texture);
	GameObject* GetObject(){ return m_object; }
	inline void SetObject(GameObject* object){ m_object = object; m_object->name = m_name; }
	inline Light* GetLight(){ return m_light; }
	inline void SetLight(Light* light){ m_light = light; }
	inline double GetSpin(){ return m_spin; }
	inline void SetSpin(double s){ m_spin = s; }
	inline double GetAngularVelocity(){ return m_angularVelocity; }
	inline void SetAngularVelocity(double v){ m_angularVelocity = v; }
	inline glm::dvec3 GetVelocity(){ return m_velocity; }
	inline void SetVelocity(const glm::dvec3 v){ m_velocity = v; }
	inline double GetMass(){ return m_mass; }
	inline void SetMass(const double m){ m_mass = m; }
	inline double GetRadius(){ return m_radius; }
	inline double GetEmission(){ return m_lightIntencity; }
	void DrawTail(Material* material, Camera& camera);
	void CalculateTail();
	void DrawPlanet(Camera& camera);
	void DrawAtmosphere(Camera& camera);
	void UpdateAtmosphereShaderUniforms(Camera& camera,Shader* shader);
	void DrawSelection(Camera& camera,Shader* shader);
	void DrawLight(Camera& camera,LightPass* lightPass);
	inline void SetOrbitColor(glm::vec3 color){ m_orbitColor = color; }
	inline void SetOrbitColor(float r, float g, float b){ m_orbitColor = glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f); }
	void SaveOrbitPoint();
	void ClearTail();
	~Planet();
private:
	std::string GetRandomName();
	double m_mass;
	glm::dvec3 m_velocity;
	double m_spin = 0;
	double m_angularVelocity;
	GameObject* m_object;
	Light* m_light = nullptr;
	std::string m_name;
	int m_orbitCounter = 0;
	double m_orbitSampleTimer = 0;
	double m_lightIntencity;
	double m_radius;
	glm::vec3 m_orbitColor;
	std::list<glm::dvec3> m_orbit;
};
#endif //PLANET_H