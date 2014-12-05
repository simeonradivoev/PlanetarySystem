#include "Planet.h"
#include "Core/Time.h"
#include "Material.h"
#include "shader.h"
#include <fstream>
#include <vector>
#include <iostream>

#define MAX_ORBIT_LENGTH 512
#define ORBIT_SAMPLE_TIME 0.1

Planet::Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius,double lightIntencity,Shader* shader,Texture* texture)
:m_name(name), m_mass(mass), m_velocity(velocity), m_angularVelocity(angularVelocity), m_radius(radius), m_lightIntencity(lightIntencity)
{
	if (name.empty())
		m_name = GetRandomName();

	Material* mat = new Material(shader);
	mat->AddTexture(texture);
	mat->SetEmission(lightIntencity);
	SetObject(new GameObject(name, GameObject::GetSphereMesh(), mat));
	GetObject()->GetTransform().scale = glm::dvec3(radius);
	m_orbitColor = glm::dvec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

	if (lightIntencity > 0)
	{
		m_light = new Light(glm::vec3(1), lightIntencity, 0, 1);
		m_light->SetAmbientIntencity(0.05);
	}
}

void Planet::DrawTail(Material* material,Camera& camera){
	material->SetColor(glm::vec4(m_orbitColor, 1));
	material->Bind();
	material->Update(Transform(), camera);
	

	glBegin(GL_LINE_STRIP);
	for (std::list<glm::dvec3>::iterator node = m_orbit.begin(); node != m_orbit.end(); node++)
	{
		glVertex3d(node->x,node->y,node->z);
	}
	glEnd();
}

void Planet::CalculateTail()
{
	m_orbitSampleTimer += Time::GetDeltaTime(TIME_DELTA_RAW) * Time::GetTimeScale();

	if (m_orbitSampleTimer > ORBIT_SAMPLE_TIME)
	{
		SaveOrbitPoint();
		m_orbitCounter++;

		while (m_orbitCounter > MAX_ORBIT_LENGTH)
		{
			m_orbit.pop_front();
			m_orbitCounter--;
		}

		m_orbitSampleTimer = 0;
	}
}

void Planet::SaveOrbitPoint()
{
	m_orbit.push_back(glm::dvec3(GetObject()->transform.position));
}

void Planet::ClearTail()
{
	m_orbit.clear();
}

void Planet::DrawPlanet(Camera& camera){
	if (m_object != nullptr){
		m_object->Draw(camera);
	}
}

void Planet::DrawSelection(Camera& camera,Shader* shader)
{
	GameObject::DrawPlane(Transform(GetObject()->GetTransform().position,glm::dquat(), GetObject()->transform.scale * 3.0), Shader::FindShader("glowShader"), camera);
}

void Planet::DrawLight(Camera& camera,LightPass* lightPass){
	if (m_light != nullptr){
		m_light->Draw(camera, lightPass);
	}
}

std::string Planet::GetRandomName()
{
	std::ifstream file;
	file.open("./res/planetNames.txt");

	std::string line;
	std::vector<std::string> names;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			names.push_back(line);
			std::cout << line << std::endl;
		}
	}

	return names[(int)(names.size() * ((double)rand() / RAND_MAX)) - 1];
}


Planet::~Planet()
{
	if (m_light != nullptr)
		delete m_light;

	if (m_object != nullptr)
	delete m_object;
}
