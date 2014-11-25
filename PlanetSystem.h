#pragma once
#include "Scene.h"
#include "Planet.h"
#include <list>
#include "glm\glm.hpp"
#include "glm\gtx\quaternion.hpp"

class PlanetSystem: public Scene
{
public:
	PlanetSystem();
	~PlanetSystem();
	void FixedUpdate() override;
	void AddPlanet(Planet* planet);
	void Draw(Camera& cam, Display& display) override;
	void Create() override;
	static const double G;
private:
	std::list<Planet*> m_planets;
	void MovePlanet(Planet* planet);
	void AddForcesToPlanet(Planet* planet);
	void DrawGrid(Camera& camera, Display& display);
	double ForceBetween(Planet* a, Planet* b);
	double round(double f, double prec);
	Material* m_orbitMaterial;
	Material* m_gridMaterial;
	Transform m_gridTransform;
};

