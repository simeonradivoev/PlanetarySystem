#pragma once
#ifndef PLANET_SYSTEM_H
#define PLANET_SYSTEM_H

#include "Scene.h"

#include <list>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>

class Material;
class Planet;
class LightPass;

class PlanetSystem: public Scene
{
public:
	PlanetSystem();
	~PlanetSystem();
	void FixedUpdate() override;
	void AddPlanet(Planet* planet);
	void GeometryPass(Camera& cam) override;
	void LightingPass(Camera& cam,LightPass* lightPass) override;
	void Create() override;
	static const double G;
private:
	std::list<Planet*> m_planets;
	void MovePlanet(Planet* planet);
	void AddForcesToPlanet(Planet* planet);
	void DrawGrid(Camera& camera);
	double ForceBetween(Planet* a, Planet* b);
	double round(double f, double prec);
	Material* m_orbitMaterial;
	Material* m_gridMaterial;
	Transform m_gridTransform;
};
#endif //PLANET_SYSTEM_H