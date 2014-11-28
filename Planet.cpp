#include "Planet.h"
#include "Time.h"
#include "Material.h"
#include "shader.h"

#include <iostream>

#define MAX_ORBIT_LENGTH 128
#define ORBIT_SAMPLE_TIME 0.5

Planet::Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius,double lightIntencity,Shader* shader,Texture* texture)
:m_name(name), m_mass(mass), m_velocity(velocity), m_angularVelocity(angularVelocity), m_radius(radius), m_lightIntencity(lightIntencity)
{
	Material* mat = new Material(shader);
	mat->AddTexture(texture);
	SetObject(GameObject::CreateSphere(radius, 5, mat));

	if (lightIntencity > 0)
	{
		m_light = new Light(glm::vec3(1), lightIntencity, 0, 0.6);
		m_light->SetAmbientIntencity(0.05);
	}
}

void Planet::DrawTail(Material* material,Camera& camera){

	m_orbitSampleTimer += Time::GetDeltaTime(TIME_DELTA_RAW);

	if (m_orbitSampleTimer > ORBIT_SAMPLE_TIME)
	{
		m_orbit.push_back(glm::dvec3(GetObject()->transform.position));
		m_orbitCounter++;
		std::cout << m_orbitCounter << std::endl;

		if (m_orbitCounter > MAX_ORBIT_LENGTH){
			m_orbit.pop_front();
			m_orbitCounter--;
		}

		m_orbitSampleTimer = 0;
	}

	material->Bind();
	material->Update(Transform(), camera);

	glBegin(GL_LINE_STRIP);
	for (std::list<glm::dvec3>::iterator node = m_orbit.begin(); node != m_orbit.end(); node++){
		glVertex3d(node->x,node->y,node->z);
	}

	glm::dvec3 planetPos = GetObject()->transform.position;
	glVertex3d(planetPos.x, planetPos.y, planetPos.z);
	glEnd();
}

void Planet::DrawPlanet(Camera& camera){
	if (m_object != nullptr){
		m_object->Draw(camera);
	}
}

void Planet::DrawLight(Camera& camera,LightPass* lightPass){
	if (m_light != nullptr){
		m_light->Draw(camera, lightPass);
	}
}


Planet::~Planet()
{
	if (m_light != 0)
		delete m_light;
}
