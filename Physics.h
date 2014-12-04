#pragma once
#ifndef PHYSICS_H
#define PHYSICS_H

#include <glm\glm.hpp>

class Physics
{
public:
	struct Ray
	{
	public:
		Ray(glm::dvec3 position,glm::dvec3 direction) :m_direction(glm::normalize(direction)),m_position(position){}

		inline glm::dvec3 GetPoint(double distance)
		{
			return m_position + m_direction * distance;
		}
		inline glm::dvec3 GetDirection(){ return m_direction; }
		inline void SetDirection(glm::dvec3 direction){ m_direction = direction; }
		inline glm::dvec3 GetPosition(){ return m_position; }
		inline void SetPosition(glm::dvec3 position){ m_position = position; }
	private:
		glm::dvec3 m_direction;
		glm::dvec3 m_position;
	};

	Physics();
	static bool IntersectsSphere(Ray ray, glm::dvec3 spherePosition, double sphereRadius, glm::dvec3 *hit);
	static bool IntersectsPlane(Ray ray, glm::dvec3 planePosition, glm::dvec3 planeNormal, glm::dvec3 *hit);
	~Physics();
};
#endif //PHYSICS_H
