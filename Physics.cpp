#include "physics.h"


Physics::Physics()
{
}

bool Physics::IntersectsSphere(Ray ray, glm::dvec3 spherePosition, double sphereRadius,glm::dvec3 *hit)
{
	double closestPointDistance = glm::dot(ray.GetDirection(), spherePosition - ray.GetPosition());
	glm::dvec3 closestPoint = ray.GetPoint(closestPointDistance);
	double d = glm::distance(closestPoint, spherePosition);

	if (d < sphereRadius)
	{
		*hit = ray.GetPoint(closestPointDistance - glm::sqrt(glm::pow(sphereRadius, 2.0) - glm::pow(d, 2.0)));
		return true;
	}
	else if (d == sphereRadius)
	{
		*hit = closestPoint;
		return true;
	}

	return false;
}

bool Physics::IntersectsPlane(Ray ray, glm::dvec3 planePosition, glm::dvec3 planeNormal,glm::dvec3 *hit)
{
	double d = glm::dot(planeNormal, ray.GetPosition() - planePosition);
	double a = glm::dot(planeNormal, ray.GetDirection());

		double distanceToHit = -d / a;
		*hit = ray.GetPoint(distanceToHit);
		return true;
}

Physics::~Physics()
{
}
