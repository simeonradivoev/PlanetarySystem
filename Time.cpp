#include "Time.h"
#include <cassert>


double Time::deltaTime;
double Time::deltaTimeSmooth;
double Time::time;
double Time::m_timeScale = 1;
const double Time::m_fixedDeltaTime = 1 / 60.0;

Time::Time()
{
	Time::deltaTime = glfwGetTime();
	lastDeltaTime = Time::deltaTime;
	maxDeltaTime = 1 / 30.0;
}


void Time::Update(Scene& scene){
	double currentTime = glfwGetTime();
	double frameTime = (currentTime - lastDeltaTime) * m_timeScale;
	lastDeltaTime = currentTime;
	accumilator += frameTime;

	while (frameTime > 0.0)
	{
		Time::deltaTime = fmin(Time::deltaTime, maxDeltaTime);
		Time::deltaTimeSmooth = glm::lerp<double>(Time::deltaTimeSmooth, Time::deltaTime, 0.1f);
		frameTime -= Time::deltaTime;
		//Time::time += Time::deltaTime;
	}

	while (accumilator >= Time::m_fixedDeltaTime)
	{
		accumilator -= Time::m_fixedDeltaTime;
		Time::time += Time::m_fixedDeltaTime;
		scene.FixedUpdate();
	}

	scene.Update();
}


Time::~Time()
{
}
