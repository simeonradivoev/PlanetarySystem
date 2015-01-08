#pragma once
#ifndef PLANET_SYSTEM_H
#define PLANET_SYSTEM_H

#include "Scene.h"
#include "texture2D.h"
#include "gui.h"

#include <list>
#include <glm\glm.hpp>
#include <glm\gtx\quaternion.hpp>
#include <queue>

#define GRID_CELLS_COUNT 256
#define ORBIT_PREDITION_STEPS 512
#define ORBIT_PREDITION_STEP_SIZE 0.2
#define MAX_PLANET_INTERACTION 16

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
	void Update() override;
	void AddPlanet(Planet* planet);
	void GeometryPass(Camera& cam) override;
	void AtmospherePass(Camera& cam);
	void TransperentPass(Camera& cam) override;
	void LightingPass(Camera& cam,LightPass* lightPass) override;
	void GUI(Canvas* canvas, Camera& camera) override;
	void SelectedPlanetOptionsWindow(Canvas* canvas, GUI::Rect rect);
	void CreateRandomPlanet();
	void Create() override;
	static const double G;
private:
	std::list<Planet*> m_planets;
	void MovePlanet(Planet* planet);
	glm::dvec3 CalculateVelocity(Planet* planet, double timeStep);
	void DrawGrid(Camera& camera);
	void DrawPlanetPlace(Camera& camera);
	void DrawPlanetPlaceHelpers(Camera& camera);
	void PredictOrbit(Planet* planet);
	void ManagePlanetPlace(Canvas* canvas);
	double ForceBetween(Planet* a, Planet* b);
	double round(double f, double prec);
	double m_speed = 0.2;
	Material* m_orbitMaterial;
	Material* m_gridMaterial;
	Transform m_gridTransform;
	Planet* m_selectedPlanet = nullptr;
	std::queue<Planet*> m_planetQueue;
	Planet* m_planetToBePlaced = nullptr;
	Texture2D m_earthTexture;
	Texture2D m_sunTexture;
	Texture2D m_jupiterTexture;
};
#endif //PLANET_SYSTEM_H