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
const double minSpeed = 0.001;
const double maxSpeed = 3;

PlanetSystem::PlanetSystem()
:m_earthTexture("./res/earth.jpg"), m_jupiterTexture("./res/jupiter.jpg"), m_sunTexture("./res/sun.jpg")
{
	Shader* orbitShader = Shader::FindShader("orbitShader");
	m_orbitMaterial = new Material(orbitShader);
	m_orbitMaterial->SetColor(glm::vec4(1, 1, 1, 0.3));
	m_orbitMaterial->SetEmission(0);
	m_gridMaterial = new Material(orbitShader);
	m_gridMaterial->SetColor(glm::vec4(0.6, 0.8, 1, 0.05));
	m_gridMaterial->SetEmission(0);
}

bool ComparePlanetsByMass(Planet* first,Planet* second)
{
	return first->GetMass() > second->GetMass();
}

void PlanetSystem::AddPlanet(Planet* planet){
	m_planets.push_back(planet);
	m_planets.sort(ComparePlanetsByMass);
}

void PlanetSystem::FixedUpdate()
{
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++){
		MovePlanet(*planet);
		planet.operator*()->CalculateTail();
		planet.operator*()->SetVelocity(CalculateVelocity(planet.operator*(),Time::GetFixedDeltaTime()));
	}

	if (m_selectedPlanet != nullptr)
	{
		Camera::GetCurrentCamera()->transform.position += m_selectedPlanet->GetVelocity() * Time::GetFixedDeltaTime();

		if (Input::GetCurrentEvent().keyAction == GLFW_REPEAT || Input::GetCurrentEvent().keyAction == GLFW_PRESS)
		{
			if (Input::GetCurrentEvent().key == GLFW_KEY_Z)
			{
				Camera::GetCurrentCamera()->transform.position = m_selectedPlanet->GetObject()->transform.position + glm::normalize(Camera::GetCurrentCamera()->transform.position - m_selectedPlanet->GetObject()->transform.position) * (m_selectedPlanet->GetRadius() + 0.5);
			}
		}
	}
}

void PlanetSystem::Update()
{
	Time::SetTimeScale(minSpeed + m_speed * (maxSpeed - minSpeed));
}

void PlanetSystem::MovePlanet(Planet* planet)
{
	double spin = planet->GetSpin();
	double angularVelocity = planet->GetAngularVelocity();
	spin += angularVelocity * Time::GetFixedDeltaTime();
	planet->SetSpin(spin);
	if (planet->GetObject() != nullptr)
	{
		planet->GetObject()->transform.rotation = glm::angleAxis(spin, glm::dvec3(0, 1, 0));
		planet->GetObject()->transform.position += planet->GetVelocity() * Time::GetFixedDeltaTime();
	}
	if (planet->GetLight() != nullptr)
	{
		planet->GetLight()->GetBoundingSphere()->transform.rotation = glm::angleAxis(spin, glm::dvec3(0, 1, 0));
		planet->GetLight()->GetBoundingSphere()->transform.position += planet->GetVelocity() * Time::GetFixedDeltaTime();
	}
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

		Planet* Jupiter = new Planet("Jupiter", 10E6, glm::dvec3(0, 0, 1), 20, 0.6, 0, planetShader, &m_jupiterTexture);
		Jupiter->GetObject()->transform.position = glm::dvec3(-8, 0, 0);
		Jupiter->SetOrbitColor(191, 127, 63);

		Planet* Earth = new Planet("Earth", 10E3, glm::dvec3(0, 0, -1), 10E3, 0.2, 0, planetShader, &m_earthTexture);
		Earth->GetObject()->transform.position = glm::dvec3(4, 0, 0);
		Earth->SetOrbitColor(62, 187, 187);

		Planet* Earth2 = new Planet("Earth 2.0", 10E3, glm::dvec3(-1, 0, 0), 10E3, 0.2, 0, planetShader, &m_earthTexture);
		Earth2->GetObject()->transform.position = glm::dvec3(0, -4, 0);
		Earth2->SetOrbitColor(127, 191, 63);

		Planet* Sun = new Planet("Sun", 10E10, glm::dvec3(0, 0, 0), 5, 1, 512, sunShader, &m_sunTexture);
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

	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		planet.operator*()->DrawPlanet(cam);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		if (planet.operator*()->GetEmission() <= 0)
			planet.operator*()->DrawAtmosphere(cam);
		glDisable(GL_BLEND);
	}

	DrawPlanetPlace(cam);
}

void PlanetSystem::AtmospherePass(Camera& cam)
{
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		glBlendFunc(GL_ONE, GL_ONE);
		if (planet.operator*()->GetEmission() <= 0)
			planet.operator*()->DrawAtmosphere(cam);
	}
}

void PlanetSystem::TransperentPass(Camera& cam)
{
	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		glBlendFunc(GL_ONE, GL_ONE);
		planet.operator*()->DrawTail(m_orbitMaterial,cam);
	}

	if (Input::MouseVisible)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_gridMaterial->GetColor().a = glm::lerp<float>(m_gridMaterial->GetColor().a, 0.05, 0.05);
		DrawGrid(cam);
	}
	else
	{
		m_gridMaterial->GetColor().a = glm::lerp<float>(m_gridMaterial->GetColor().a, 0, 0.05);
	}

	DrawPlanetPlaceHelpers(cam);
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

	m_gridTransform.position.x = round(camera.transform.position.x, CellSize);
	m_gridTransform.position.z = round(camera.transform.position.z, CellSize);
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

void PlanetSystem::ManagePlanetPlace(Canvas* canvas)
{
	if (!m_planetQueue.empty() && Input::MouseVisible && m_planetToBePlaced == nullptr)
	{
		m_planetQueue.front()->GetObject()->transform.position = Input::GetMouseWorldPlanePosition();

		if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1 && !canvas->GetCurrentEvent().Events[CE_PRESS])
		{
			m_planetToBePlaced = m_planetQueue.front();
			m_planetQueue.pop();
		}

		if (Input::GetCurrentEvent().keyAction == GLFW_PRESS && Input::GetCurrentEvent().key == GLFW_KEY_ESCAPE)
		{
			delete m_planetQueue.front();
			m_planetQueue.pop();
			Input::TakeCurrentEvent();
		}

	}
	else if (Input::MouseVisible && m_planetToBePlaced != nullptr)
	{
		if (Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1 && !canvas->GetCurrentEvent().Events[CE_PRESS])
		{
			if (Input::GetCurrentEvent().mouseAction == GLFW_DRAGG)
			{
				PredictOrbit(m_planetToBePlaced);
				m_planetToBePlaced->SetVelocity(Input::GetMouseWorldPlanePosition() - m_planetToBePlaced->GetObject()->transform.position);

				if (Input::GetCurrentEvent().keyAction == GLFW_PRESS && Input::GetCurrentEvent().key == GLFW_KEY_ESCAPE)
				{
					delete m_planetToBePlaced;
					m_planetToBePlaced = nullptr;
					Input::TakeCurrentEvent();
				}
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
}

void PlanetSystem::DrawPlanetPlaceHelpers(Camera& camera)
{
	if (Input::MouseVisible && m_planetToBePlaced != nullptr)
	{
		m_planetToBePlaced->DrawPlanet(camera);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		m_planetToBePlaced->DrawTail(m_orbitMaterial, camera);
		GUI::DrawLine(m_planetToBePlaced->GetObject()->transform.position, Input::GetMouseWorldPlanePosition(), glm::vec4(1, 0, 0, 1), Shader::FindShader("orbitShader"));
	}
}

GUI::Rect SelectedPlanetWindowRect = GUI::Rect(64, 256, 256, 256);
double speed = 0.2;

void PlanetSystem::GUI(Canvas* canvas,Camera& camera)
{
	Physics::Ray mouseRay = Camera::GetCurrentCamera()->ScreenToRay(Input::GetMousePosition());
	glm::dvec3 planeHit;
	Physics::IntersectsPlane(mouseRay, glm::dvec3(10, 0, 10), glm::dvec3(0, 10, 0), &planeHit);

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
		std::ostringstream speedLabelText;
		speedLabelText << std::setprecision(3) << Time::GetTimeScale();
		GUI::Label(canvas, GUI::Rect(64, 8, 128, 32), speedLabelText.str().c_str(), "label");
		m_speed = GUI::Slider(canvas, GUI::Rect(256, 8, 512, 32), m_speed, "slider");

		if (m_planetQueue.size() > 0)
			GUI::Label(canvas, GUI::Rect(width - 86, height / 2 - 12, 12, 12),std::to_string(m_planetQueue.size()).c_str(),"label");

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

	for (std::list<Planet*>::iterator planet = m_planets.begin(); planet != m_planets.end(); planet++)
	{
		glm::dvec3 planetHit;

		if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1 && !canvas->GetCurrentEvent().Events[CE_PRESS])
		{
			if ((Physics::IntersectsSphere(mouseRay, planet.operator*()->GetObject()->transform.GetPos(), planet.operator*()->GetRadius(), &planetHit) && Input::MouseVisible))
			{

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

	ManagePlanetPlace(canvas);

	//enable and disable the mouse
	if (Input::GetCurrentEvent().keyAction == GLFW_PRESS && Input::GetCurrentEvent().key == GLFW_KEY_ESCAPE && canvas->GetActiveControl() == NULL && !Input::CurrentEventTaken())
	{
		if (Input::MouseVisible)
		{
			glfwSetInputMode(Display::GetCurrentDisplay()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			Input::MouseVisible = false;
		}
		else
		{
			glfwSetInputMode(Display::GetCurrentDisplay()->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			Input::MouseVisible = true;
		}
	}
}

std::string test = "test";

void PlanetSystem::SelectedPlanetOptionsWindow(Canvas* canvas, GUI::Rect rect)
{
	Canvas::Layout l = canvas->GetCurrentLayout();

	GUI::Style nameStyle = canvas->GetStyle("label");
	nameStyle.fontSize = 40;
	GUI::Label(canvas, GUI::Rect(0, 0, l.width, 40), m_selectedPlanet->GetObject()->name.c_str(), nameStyle);
	//mass
	std::ostringstream mass;
	mass << m_selectedPlanet->GetMass() << " kg.";
	GUI::Label(canvas, GUI::Rect(0, 0 + 40, l.width, 20), ("mass: " + mass.str()).c_str(), "label");
	//radius
	std::ostringstream radius;
	radius << "radius: " << m_selectedPlanet->GetRadius();
	GUI::Label(canvas, GUI::Rect(0, 0 + 60, l.width, 20), radius.str().c_str(), "label");
	//speed
	std::ostringstream speed;
	speed << std::setprecision(3) << "speed: " << glm::length(m_selectedPlanet->GetVelocity());
	GUI::Label(canvas, GUI::Rect(0, 0 + 80, l.width, 20), speed.str().c_str(), "label");
	//distance
	std::ostringstream distance;
	distance << std::setprecision(4) << "distance form center: " << glm::length(m_selectedPlanet->GetObject()->transform.position);
	GUI::Label(canvas, GUI::Rect(0, 0 + 100, l.width, 20), distance.str().c_str(), "label");

	m_selectedPlanet->GetObject()->name = GUI::TextField(canvas, GUI::Rect(0, 0 + 130, l.width, 32), m_selectedPlanet->GetObject()->name, "textField");
	
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
