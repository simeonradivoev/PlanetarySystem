#include "Time.h"
#include "Input.h"

double Time::deltaTime;
double Time::deltaTimeSmooth;
double Time::time;
double Time::m_timeScale = 1;
double Time::m_fixedDeltaTime = 1 / 60.0;
double Time::m_deltaTimeUnscaled = 0;
double Time::m_timeUnscaled = 0;

#define FIXED_DELTA_TIME 1.0 / 60.0

Time::Time()
{
	Time::deltaTime = glfwGetTime();
	lastDeltaTime = Time::deltaTime;
	maxDeltaTime = 1 / 30.0;
}


void Time::Update(Scene& scene){

	Time::m_timeScale = glm::lerp(Time::m_timeScale,Input::MouseVisible ? 0.1 : 1,0.05);

	double currentTime = glfwGetTime();
	Time::deltaTime = (currentTime - lastDeltaTime) * Time::m_timeScale;
	Time::m_deltaTimeUnscaled = currentTime - lastDeltaTime;
	Time::m_fixedDeltaTime = FIXED_DELTA_TIME * Time::m_timeScale;
	lastDeltaTime = currentTime;
	accumilator += Time::deltaTime;

	Time::deltaTimeSmooth = glm::lerp<double>(Time::deltaTimeSmooth, Time::deltaTime, 0.1f);
	Time::time += Time::deltaTime;
	Time::m_timeUnscaled += Time::m_deltaTimeUnscaled;

	while (accumilator >= Time::m_fixedDeltaTime)
	{
		accumilator -= Time::m_fixedDeltaTime;
		
		scene.FixedUpdate();
	}

	scene.Update();
}


Time::~Time()
{
}
