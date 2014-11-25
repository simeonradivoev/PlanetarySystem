#pragma once
#include "GameObject.h"
#include "Material.h"
#include <string>
#include <list>

class Planet
{
public:
	Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius);
	GameObject* GetObject(){ return m_object; }
	void SetObject(GameObject* object){ m_object = object; }
	double GetSpin(){ return m_spin; }
	void SetSpin(double s){ m_spin = s; }
	double GetAngularVelocity(){ return m_angularVelocity; }
	void SetAngularVelocity(double v){ m_angularVelocity = v; }
	glm::dvec3 GetVelocity(){ return m_velocity; }
	void SetVelocity(const glm::dvec3 v){ m_velocity = v; }
	double GetMass(){ return m_mass; }
	void SetMass(const double m){ m_mass = m; }
	void DrawTail(Material* material, const Camera& camera, Display& display);
	~Planet();
private:
	double m_mass;
	glm::dvec3 m_velocity;
	double m_spin = 0;
	double m_angularVelocity;
	GameObject* m_object;
	std::string m_name;
	int m_orbitCounter = 0;
	double m_orbitSampleTimer = 0;
	std::list<glm::dvec3> m_orbit;
};

