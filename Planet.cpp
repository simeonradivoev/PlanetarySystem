#include "Planet.h"
#include <iostream>
#include "Time.h"

#define MAX_ORBIT_LENGTH 128
#define ORBIT_SAMPLE_TIME 0.5

Planet::Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius)
{
	m_name = name;
	m_mass = mass;
	m_velocity = velocity;
	m_angularVelocity = angularVelocity;
}

void Planet::DrawTail(Material* material,const Camera& camera,Display& display){

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
	material->Update(Transform(), camera, display);

	glBegin(GL_LINE_STRIP);
	for (std::list<glm::dvec3>::iterator node = m_orbit.begin(); node != m_orbit.end(); node++){
		glVertex3d(node->x,node->y,node->z);
	}

	glm::dvec3 planetPos = GetObject()->transform.position;
	glVertex3d(planetPos.x, planetPos.y, planetPos.z);
	glEnd();
}


Planet::~Planet()
{
}
