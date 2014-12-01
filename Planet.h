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
	void SetObject(GameObject* object){ m_object = object; m_object->name = m_name; }
	Light* GetLight(){ return m_light; }
	void SetLight(Light* light){ m_light = light; }
	double GetSpin(){ return m_spin; }
	void SetSpin(double s){ m_spin = s; }
	double GetAngularVelocity(){ return m_angularVelocity; }
	void SetAngularVelocity(double v){ m_angularVelocity = v; }
	glm::dvec3 GetVelocity(){ return m_velocity; }
	void SetVelocity(const glm::dvec3 v){ m_velocity = v; }
	double GetMass(){ return m_mass; }
	void SetMass(const double m){ m_mass = m; }
	double GetRadius(){ return m_radius; }
	void DrawTail(Material* material, Camera& camera);
	void CalculateTail();
	void DrawPlanet(Camera& camera);
	void DrawLight(Camera& camera,LightPass* lightPass);
	~Planet();
private:
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
	std::list<glm::dvec3> m_orbit;
};
#endif //PLANET_H