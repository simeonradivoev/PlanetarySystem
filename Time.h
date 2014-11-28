#pragma once
#ifndef TIME_H
#define TIME_H

#include "Scene.h"
#include <glm\glm.hpp>
#include <glm\gtx\compatibility.hpp>
#include <GLFW\glfw3.h>

#define TIME_DLETA_SMOOTH true
#define TIME_DELTA_RAW false

class Time
{
public:
	Time();
	~Time();
	static void SetTimeScale(double timeScale){ Time::m_timeScale; }
	static double GetTimeScale(){ return m_timeScale; }
	static const double GetFixedDeltaTime(){ return m_fixedDeltaTime; }
	static const double GetDeltaTime(bool type){ if (type) return Time::deltaTimeSmooth; else return Time::deltaTime; }
	static const double GetTime(){ return time; }
	void Update(Scene& scene);
private:
	double lastDeltaTime;
	double maxDeltaTime;
	double accumilator = 0.0;

	static double time;
	const static double m_fixedDeltaTime;
	static double deltaTime;
	static double deltaTimeSmooth;
	static double m_timeScale;
};
#endif //TIME_H