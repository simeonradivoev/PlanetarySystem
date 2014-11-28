#include "PlanetSystem.h"
#include "Time.h"
#include "Planet.h"
#include "shader.h"
#include "Material.h"
#include "texture2D.h"

#include <iostream>
#include <glm\gtc\type_ptr.hpp>

const double PlanetSystem::G = 6.67384E-11;

PlanetSystem::PlanetSystem()
{
	Shader* orbitShader = new Shader("./res/orbitShader");
	m_orbitMaterial = new Material(orbitShader);
	m_orbitMaterial->SetColor(glm::vec4(0.2, 1, 0.2, 0.2));
	m_gridMaterial = new Material(orbitShader);
	m_gridMaterial->SetColor(glm::vec4(0.6, 0.8, 1, 0.05));

}

void PlanetSystem::AddPlanet(Planet* planet){
	m_planets.push_back(planet);
}

void PlanetSystem::FixedUpdate(){
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		MovePlanet(*planet);
		AddForcesToPlanet(*planet);
	}
}

void PlanetSystem::MovePlanet(Planet* planet){
	double spin = planet->GetSpin();
	double angularVelocity = planet->GetAngularVelocity();
	spin += angularVelocity * Time::GetFixedDeltaTime();
	planet->SetSpin(spin);
	planet->GetObject()->transform.rotation = glm::angleAxis(spin, glm::dvec3(0, 1, 0));
	planet->GetObject()->transform.position += planet->GetVelocity() * Time::GetFixedDeltaTime();
}

void PlanetSystem::AddForcesToPlanet(Planet* planet){
	glm::dvec3 velocity = planet->GetVelocity();

	//velocity += glm::normalize(-planet->GetObject()->transform.position) * Time::GetFixedDeltaTime();

	for (std::list<Planet*>::iterator neighbor = m_planets.begin(); neighbor != m_planets.end(); neighbor++){
		if (neighbor.operator*() != planet){
			velocity += glm::normalize(neighbor.operator*()->GetObject()->transform.position - planet->GetObject()->transform.position) * Time::GetFixedDeltaTime() * ForceBetween(neighbor.operator*(), planet) / planet->GetMass();
		}
	}
	planet->SetVelocity(velocity);
}

double PlanetSystem::ForceBetween(Planet* a, Planet* b){
	return PlanetSystem::G * a->GetMass() * b->GetMass() / glm::distance2(a->GetObject()->transform.position,b->GetObject()->transform.position);
}

void PlanetSystem::Create(){
	if (CurrentScene != NULL){
		Shader* planetShader = new Shader("./res/geometryPass");
		Shader* sunShader = new Shader("./res/geometryPass");

		Planet* Earth = new Planet("Earth", 10E3, glm::dvec3(0, 0, -1), 10E3, 0.2, 0, planetShader, new Texture2D("./res/earth.jpg"));
		Earth->GetObject()->transform.position = glm::dvec3(4, 0, 0);

		Planet* Jupiter = new Planet("Jupiter", 10E6, glm::dvec3(0, 0, 1), 20, 0.6, 0, planetShader, new Texture2D("./res/jupiter.jpg"));
		Jupiter->GetObject()->transform.position = glm::dvec3(-8, 0, 0);

		Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 1, sunShader, new Texture2D("./res/sun.jpg"));
		Sun->GetObject()->transform.position = glm::dvec3(0, 0, 0);

		AddPlanet(Jupiter);
		AddPlanet(Earth);
		AddPlanet(Sun);
	}
}

void PlanetSystem::GeometryPass(Camera& cam){
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		planet.operator*()->DrawPlanet(cam);
		planet.operator*()->DrawTail(m_orbitMaterial, cam);
	}

	DrawGrid(cam);
}

void PlanetSystem::LightingPass(Camera& cam,LightPass* lightPass)
{
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		planet.operator*()->DrawLight(cam, lightPass);
	}
}

void PlanetSystem::DrawGrid(Camera& camera){

	unsigned int Cells = 256;
	float CellSize = 0.2f;

	m_gridTransform.position.x = -round(camera.transform.position.x, CellSize);
	m_gridTransform.position.z = -round(camera.transform.position.z, CellSize);
	//std::cout << camera.transform.position.y << std::endl;
	m_gridMaterial->Bind();
	m_gridMaterial->Update(m_gridTransform, camera);

	
	for (unsigned int x = 0; x < Cells; x++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3d((Cells * CellSize) / -2.0, 0, (Cells * CellSize) / -2.0 + x * CellSize);
		glVertex3d((Cells * CellSize) / 2.0, 0, (Cells * CellSize) / -2.0 + x * CellSize);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3d((Cells * CellSize) / -2.0 + x * CellSize, 0, (Cells * CellSize) / -2.0);
		glVertex3d((Cells * CellSize) / -2.0 + x * CellSize, 0, (Cells * CellSize) / 2.0);
		glEnd();
	}
	
}

double PlanetSystem::round(double f, double prec)
{
	return (double)(floor(f*(1.0 / prec) + 0.5) / (1.0 / prec));
}


PlanetSystem::~PlanetSystem()
{
}
