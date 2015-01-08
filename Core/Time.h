#pragma once
#ifndef TIME_H
#define TIME_H

#include "../Scene.h"
#include <glm\glm.hpp>
#include <glm\gtx\compatibility.hpp>
#include <GLFW\glfw3.h>

#define TIME_DLETA_SMOOTH 0
#define TIME_DELTA_RAW 1
#define TIME_DELTA_UNSCALSED 2


class Time
{
public:
	Time();
	~Time();
	static void SetTimeScale(double timeScale){ Time::m_timeScale = timeScale; }
	static double GetTimeScale(){ return m_timeScale; }
	static const double GetFixedDeltaTime(){ return m_fixedDeltaTime * m_timeScale; }
	static const double GetDeltaTime(int type){ if (type == TIME_DLETA_SMOOTH) return Time::deltaTimeSmooth; else if (type == TIME_DELTA_RAW) return Time::deltaTime; else return m_deltaTimeUnscaled; }
	static const double GetTime(){ return time; }
	static const double GetTimeUnscaled(){ return m_timeUnscaled; }
	static const int GetFPS(){ return roundEven(1.0 / Time::m_deltaTimeUnscaled); }
	void Update(Scene& scene);
private:
	double lastDeltaTime;
	double maxDeltaTime;
	double accumilator = 0.0;

	static double time;
	static double m_timeUnscaled;
	static double m_fixedDeltaTime;
	static double deltaTime;
	static double m_deltaTimeUnscaled;
	static double deltaTimeSmooth;
	static double m_timeScale;
};
#endif //TIME_H