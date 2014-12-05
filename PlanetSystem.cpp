#include "PlanetSystem.h"
#include "Core/Time.h"
#include "Planet.h"
#include "shader.h"
#include "Material.h"
#include "texture2D.h"
#include "Core\Input.h"
#include "physics.h"
#include "canvas.h"
#include <sstream>
#include <iomanip>

#include <iostream>
#include <glm\gtc\type_ptr.hpp>

const double PlanetSystem::G = 6.67384E-11;

PlanetSystem::PlanetSystem()
:m_earthTexture("./res/earth.jpg")
{
	Shader* orbitShader = Shader::FindShader("orbitShader");
	m_orbitMaterial = new Material(orbitShader);
	m_orbitMaterial->SetColor(glm::vec4(1, 1, 1, 0.3));
	m_orbitMaterial->SetEmission(1);
	m_gridMaterial = new Material(orbitShader);
	m_gridMaterial->SetColor(glm::vec4(1, 1, 1, 0.05));
	m_gridMaterial->SetEmission(1);
}

bool ComparePlanetsByMass(Planet* first,Planet* second)
{
	return first->GetMass() > second->GetMass();
}

void PlanetSystem::AddPlanet(Planet* planet){
	m_planets.push_back(planet);
	m_planets.sort(ComparePlanetsByMass);
}

void PlanetSystem::FixedUpdate(){
	Physics::Ray mouseRay = Camera::GetCurrentCamera()->ScreenToRay(Input::GetMousePosition());
	glm::dvec3 planeHit;
	Physics::IntersectsPlane(mouseRay, glm::dvec3(10, 0, 10), glm::dvec3(0, 10, 0), &planeHit);

	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		MovePlanet(*planet);
		planet.operator*()->CalculateTail();
		planet.operator*()->SetVelocity(CalculateVelocity(planet.operator*(),Time::GetFixedDeltaTime()));

		glm::dvec3 planetHit;

		if ((Physics::IntersectsSphere(mouseRay, planet.operator*()->GetObject()->transform.GetPos(), planet.operator*()->GetRadius(), &planetHit) && Input::MouseVisible))
		{
			if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1){
				if (planet.operator*() != m_selectedPlanet)
				{
					m_selectedPlanet = planet.operator*();
				}
				else
				{
					m_selectedPlanet = nullptr;
				}
			}
				
		}
	}

	ManagePlanetPlace();
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

glm::dvec3 PlanetSystem::CalculateVelocity(Planet* planet,double timeStep)
{
	glm::dvec3 velocity = planet->GetVelocity();
	int count = 0;

	for (std::list<Planet*>::iterator neighbor = m_planets.begin(); neighbor != m_planets.end(); neighbor++)
	{
		if (count > MAX_PLANET_INTERACTION)
			break;

		if (neighbor.operator*() != planet)
		{
			count++;
			velocity += glm::normalize(neighbor.operator*()->GetObject()->transform.position - planet->GetObject()->transform.position) * timeStep * ForceBetween(neighbor.operator*(), planet) / planet->GetMass();
		}
	}

	return velocity;
}

double PlanetSystem::ForceBetween(Planet* a, Planet* b)
{
	return PlanetSystem::G * a->GetMass() * b->GetMass() / glm::distance2(a->GetObject()->transform.position,b->GetObject()->transform.position);
}

void PlanetSystem::Create()
{
	if (CurrentScene != NULL)
	{
		Shader* planetShader = Shader::FindShader("geometryPass");
		Shader* sunShader = Shader::FindShader("geometryPass");

		Planet* Jupiter = new Planet("Jupiter", 10E6, glm::dvec3(0, 0, 1), 20, 0.6, 0, planetShader, new Texture2D("./res/jupiter.jpg"));
		Jupiter->GetObject()->transform.position = glm::dvec3(-8, 0, 0);
		Jupiter->SetOrbitColor(191, 127, 63);

		Planet* Earth = new Planet("Earth", 10E3, glm::dvec3(0, 0, -1), 10E3, 0.2, 0, planetShader, &m_earthTexture);
		Earth->GetObject()->transform.position = glm::dvec3(4, 0, 0);
		Earth->SetOrbitColor(62, 187, 187);

		Planet* Earth2 = new Planet("Earth 2.0", 10E3, glm::dvec3(-1, 0, 0), 10E3, 0.2, 0, planetShader, &m_earthTexture);
		Earth2->GetObject()->transform.position = glm::dvec3(0, -4, 0);
		Earth2->SetOrbitColor(127, 191, 63);

		Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 80, sunShader, new Texture2D("./res/sun.jpg"));
		Sun->GetObject()->transform.position = glm::dvec3(0, 0, 0);
		Sun->SetOrbitColor(191, 63, 63);

		AddPlanet(Jupiter);
		AddPlanet(Earth2);
		AddPlanet(Earth);
		AddPlanet(Sun);
	}
}

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void PlanetSystem::CreateRandomPlanet()
{
	double angularVelocity = fRand(0, 10E1);
	double radiusM = (double)rand() / RAND_MAX;
	double radius = 0.1 + radiusM * (0.7 - 0.1);
	double mass = 10 + radiusM * (10E8 - 10);

	Planet* planet = new Planet("", mass, glm::dvec3(0, 0, 0), angularVelocity, radius, 0, Shader::FindShader("geometryPass"), &m_earthTexture);
	m_planetQueue.push(planet);
}

void PlanetSystem::GeometryPass(Camera& cam){
	//Physics::Ray mouseRay = cam.ScreenToRay(Input::GetMousePosition());

	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		planet.operator*()->DrawPlanet(cam);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		planet.operator*()->DrawTail(m_orbitMaterial,cam);
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

	DrawPlanetPlace(cam);
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

void PlanetSystem::PredictOrbit(Planet* planet)
{
	planet->ClearTail();
	glm::dvec3 startingPos = planet->GetObject()->transform.position;
	glm::dvec3 velocity;

	for (int steps = 0; steps < ORBIT_PREDITION_STEPS; steps++)
	{
		velocity = CalculateVelocity(planet, ORBIT_PREDITION_STEP_SIZE);
		planet->SetVelocity(velocity);
		planet->GetObject()->transform.position += velocity * ORBIT_PREDITION_STEP_SIZE;
		planet->SaveOrbitPoint();
	}

	planet->GetObject()->transform.position = startingPos;
}

void PlanetSystem::ManagePlanetPlace()
{
	if (!m_planetQueue.empty() && Input::MouseVisible && m_planetToBePlaced == nullptr)
	{
		m_planetQueue.front()->GetObject()->transform.position = Input::GetMouseWorldPlanePosition();

		if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1)
		{
			m_planetToBePlaced = m_planetQueue.front();
			m_planetQueue.pop();
		}
	}
	else if (Input::MouseVisible && m_planetToBePlaced != nullptr)
	{
		if (Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1)
		{
			if (Input::GetCurrentEvent().mouseAction == GLFW_DRAGG)
			{
				PredictOrbit(m_planetToBePlaced);
				m_planetToBePlaced->SetVelocity(Input::GetMouseWorldPlanePosition() - m_planetToBePlaced->GetObject()->transform.position);
			}
			else if (Input::GetCurrentEvent().mouseAction == GLFW_RELEASE)
			{
				m_planetToBePlaced->ClearTail();
				AddPlanet(m_planetToBePlaced);
				m_planetToBePlaced = nullptr;
			}
		}
	}
}

void PlanetSystem::DrawPlanetPlace(Camera& camera)
{
	if (!m_planetQueue.empty() && Input::MouseVisible && m_planetToBePlaced == nullptr)
	{
		m_planetQueue.front()->DrawPlanet(camera);
	}
	else if (Input::MouseVisible && m_planetToBePlaced != nullptr)
	{
		m_planetToBePlaced->DrawPlanet(camera);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_planetToBePlaced->DrawTail(m_orbitMaterial,camera);
		glDisable(GL_BLEND);
		GUI::DrawLine(m_planetToBePlaced->GetObject()->transform.position,Input::GetMouseWorldPlanePosition(),glm::vec4(1,0,0,1),Shader::FindShader("orbitShader"));
	}
}

GUI::Rect SelectedPlanetWindowRect = GUI::Rect(64, 256, 256, 256);

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
			std::string name = planet.operator*()->GetObject()->name;
			if (planet.operator*() == m_selectedPlanet)
			{
				name.append("]");
				name.insert(0, "[");
			}
			GUI::Label(canvas, GUI::Rect(pos.x, pos.y - 16, 128, 32), name.c_str());
		}
	}

	if (Input::MouseVisible)
	{
		if (GUI::Button(canvas, GUI::Rect(width - 86, height / 2 - 48, 64, 64), "", "createPlanetButton"))
		{
			CreateRandomPlanet();
		}
		GUI::Button(canvas, GUI::Rect(width - 86, height / 2 + 48, 64, 64), "", "createSunButton");

		if (m_selectedPlanet != nullptr)
		{
			SelectedPlanetWindowRect = GUI::BeginWindow(canvas, 0, SelectedPlanetWindowRect, m_selectedPlanet->GetObject()->name.c_str(), "window");
			SelectedPlanetOptionsWindow(canvas, SelectedPlanetWindowRect);
			GUI::EndWindow(canvas);
		}
	}
}

void PlanetSystem::SelectedPlanetOptionsWindow(Canvas* canvas, GUI::Rect rect)
{
	GUI::Style nameStyle = canvas->GetStyle("label");
	nameStyle.fontSize = 40;
	GUI::Label(canvas, GUI::Rect(0, 0, rect.height, 40), m_selectedPlanet->GetObject()->name.c_str(), nameStyle);
	//mass
	std::ostringstream mass;
	mass << m_selectedPlanet->GetMass() << " kg.";
	GUI::Label(canvas, GUI::Rect(0, 0 + 40, rect.height, 20), ("mass: " + mass.str()).c_str(), "label");
	//radius
	std::ostringstream radius;
	radius << "radius: " << m_selectedPlanet->GetRadius();
	GUI::Label(canvas, GUI::Rect(0, 0 + 60, rect.height, 20), radius.str().c_str(), "label");
	//speed
	std::ostringstream speed;
	speed << std::setprecision(3) << "speed: " << glm::length(m_selectedPlanet->GetVelocity());
	GUI::Label(canvas, GUI::Rect(0, 0 + 80, rect.height, 20), speed.str().c_str(), "label");
	//distance
	std::ostringstream distance;
	distance << std::setprecision(4) << "distance form center: " << glm::length(m_selectedPlanet->GetObject()->transform.position);
	GUI::Label(canvas, GUI::Rect(0, 0 + 100, rect.height, 20), distance.str().c_str(), "label");
	
}


double PlanetSystem::round(double f, double prec)
{
	return (double)(floor(f*(1.0 / prec) + 0.5) / (1.0 / prec));
}


PlanetSystem::~PlanetSystem()
{
	while (!m_planets.empty())
	{
		delete m_planets.front(), m_planets.pop_front();
	}
}
