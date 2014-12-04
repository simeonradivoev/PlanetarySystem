#pragma once
#ifndef PLANET_SYSTEM_H
#define PLANET_SYSTEM_H

#include "Scene.h"

#include <list>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include "gui.h"

#define GRID_CELLS_COUNT 256

class Material;
class Planet;
class LightPass;
class Canvas;
class Shader;

class PlanetSystem: public Scene
{
public:
	PlanetSystem();
	~PlanetSystem();
	void FixedUpdate() override;
	void AddPlanet(Planet* planet);
	void GeometryPass(Camera& cam) override;
	void LightingPass(Camera& cam,LightPass* lightPass) override;
	void GUI(Canvas* canvas, Camera& camera) override;
	void SelectedPlanetOptionsWindow(Canvas* canvas, GUI::Rect rect);
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
	Planet* m_selectedPlanet = nullptr;
};
#endif //PLANET_SYSTEM_H