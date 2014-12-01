#include "PlanetSystem.h"
#include "Core/Time.h"
#include "Planet.h"
#include "shader.h"
#include "Material.h"
#include "texture2D.h"
#include "Core\Input.h"
#include "gui.h"

#include <iostream>
#include <glm\gtc\type_ptr.hpp>

const double PlanetSystem::G = 6.67384E-11;

PlanetSystem::PlanetSystem()
{
	Shader* orbitShader = new Shader("./res/orbitShader");
	m_orbitMaterial = new Material(orbitShader);
	m_orbitMaterial->SetColor(glm::vec4(1, 1, 1, 0.3));
	m_orbitMaterial->SetEmission(1);
	m_gridMaterial = new Material(orbitShader);
	m_gridMaterial->SetColor(glm::vec4(1, 1, 1, 0.05));
	m_gridMaterial->SetEmission(1);
}

void PlanetSystem::AddPlanet(Planet* planet){
	m_planets.push_back(planet);
}

void PlanetSystem::FixedUpdate(){
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		MovePlanet(*planet);
		planet.operator*()->CalculateTail();
		AddForcesToPlanet(*planet);
	}
}

void PlanetSystem::MovePlanet(Planet* planet)
{
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
	if (CurrentScene != NULL)
	{
		Shader* planetShader = new Shader("./res/geometryPass");
		Shader* sunShader = new Shader("./res/geometryPass");

		Planet* Jupiter = new Planet("Jupiter", 10E6, glm::dvec3(0, 0, 1), 20, 0.6, 0, planetShader, new Texture2D("./res/jupiter.jpg"));
		Jupiter->GetObject()->transform.position = glm::dvec3(-8, 0, 0);

		Planet* Earth = new Planet("Earth", 10E3, glm::dvec3(0, 0, -1), 10E3, 0.2, 0, planetShader, new Texture2D("./res/earth.jpg"));
		Earth->GetObject()->transform.position = glm::dvec3(4, 0, 0);

		Planet* Earth2 = new Planet("Earth 2.0", 10E3, glm::dvec3(-1, 0, 0), 10E3, 0.2, 0, planetShader, new Texture2D("./res/earth.jpg"));
		Earth2->GetObject()->transform.position = glm::dvec3(0, -4, 0);

		Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 80, sunShader, new Texture2D("./res/sun.jpg"));
		Sun->GetObject()->transform.position = glm::dvec3(0, 0, 0);

		AddPlanet(Jupiter);
		AddPlanet(Earth2);
		AddPlanet(Earth);
		AddPlanet(Sun);
	}
}

void PlanetSystem::GeometryPass(Camera& cam){
	
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		planet.operator*()->DrawPlanet(cam);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		planet.operator*()->DrawTail(m_orbitMaterial, cam);
		glDisable(GL_BLEND);
	}

	if (Input::MouseVisible)
	{
		m_gridMaterial->GetColor().a = glm::lerp<float>(m_gridMaterial->GetColor().a, 0.05, 0.05);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		DrawGrid(cam);
		glDisable(GL_BLEND);
	}
	else
	{
		m_gridMaterial->GetColor().a = glm::lerp<float>(m_gridMaterial->GetColor().a, 0, 0.05);
	}
}

void PlanetSystem::LightingPass(Camera& cam,LightPass* lightPass)
{
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		planet.operator*()->DrawLight(cam, lightPass);
	}
}

void PlanetSystem::DrawGrid(Camera& camera){

	float CellSize = 0.2;

	m_gridTransform.position.x = -round(camera.transform.position.x, CellSize);
	m_gridTransform.position.z = -round(camera.transform.position.z, CellSize);
	//std::cout << camera.transform.position.y << std::endl;
	m_gridMaterial->Bind();
	m_gridMaterial->Update(m_gridTransform, camera);
	
	for (unsigned int x = 0; x < GRID_CELLS_COUNT; x++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3d((GRID_CELLS_COUNT * CellSize) / -2.0, 0, (GRID_CELLS_COUNT * CellSize) / -2.0 + x * CellSize);
		glVertex3d((GRID_CELLS_COUNT * CellSize) / 2.0, 0, (GRID_CELLS_COUNT * CellSize) / -2.0 + x * CellSize);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3d((GRID_CELLS_COUNT * CellSize) / -2.0 + x * CellSize, 0, (GRID_CELLS_COUNT * CellSize) / -2.0);
		glVertex3d((GRID_CELLS_COUNT * CellSize) / -2.0 + x * CellSize, 0, (GRID_CELLS_COUNT * CellSize) / 2.0);
		glEnd();
	}
}

void PlanetSystem::GUI(Canvas* canvas,Camera& camera)
{
	int width, height;
	width = Display::GetCurrentDisplay()->GetWidth();
	height = Display::GetCurrentDisplay()->GetHeight();

	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		if (GUI::WorlPointOnScreen(planet.operator*()->GetObject()->transform.position + glm::dvec3(0, planet.operator*()->GetRadius(), 0), camera))
		{
			glm::vec2 pos = GUI::WorldToScreen(planet.operator*()->GetObject()->transform.position + glm::dvec3(0, planet.operator*()->GetRadius(), 0), camera);
			GUI::Label(canvas, GUI::Rect(pos.x, pos.y - 16, 128, 32), planet.operator*()->GetObject()->name.c_str());
		}
	}
}

double PlanetSystem::round(double f, double prec)
{
	return (double)(floor(f*(1.0 / prec) + 0.5) / (1.0 / prec));
}


PlanetSystem::~PlanetSystem()
{
}
