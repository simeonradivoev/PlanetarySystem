#pragma once
#ifndef GUI_FUNCTIONS
#define GUI_FUNCTIONS

#include "Core\Input.h"
#include "nanovg.h"
#include "camera.h"
#include <glm\glm.hpp>

class Canvas;
class Shader;

class GUI
{
public:
	
	struct Rect
	{
	public:
		Rect(float x, float y, float width, float height) :x(x), y(y), width(width), height(height){};

		float xmin(){ return x; }
		float xmax(){ return x + width; }
		float ymin(){ return y; }
		float ymax(){ return y + height; }

		float x;
		float y;
		float width;
		float height;
	};

	typedef void(*window_function)(void* ,Canvas*,GUI::Rect);
	struct Style
	{
		float topMargin = 0;
		float bottomMargin = 0;
		float leftMargin = 0;
		float rightMargin = 0;

		int imageHandler = -1;
		float imageAlpha = 0.8;

		char *font = "roboto";
		int textAllignment = NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE;
		float fontSize = 20.0;
		NVGcolor textColor = nvgRGBAf(1, 1, 1, 0.8);
		NVGcolor normalFillColor = nvgRGBAf(0, 0, 0, 0.1);
		NVGcolor overFillColor = nvgRGBAf(0, 0, 0, 0.3);
		NVGcolor pressFillColor = nvgRGBAf(0, 0.5, 0, 0.1);

		void SetMargin(float m){ topMargin = m; bottomMargin = m; rightMargin = m; leftMargin = m; }
		void SetMargin(float t, float b, float l, float r){ topMargin = t; bottomMargin = b; leftMargin = l; rightMargin = r; }
	};


	static bool pnpoly(int nvert, glm::vec2 *vert, glm::vec2 test);
	static bool pointInsideRect(Rect rect, glm::vec2 point);
	static bool OverRect(Rect r);
	static glm::vec2 WorldToScreen(glm::dvec3 worldPos, Camera& camera);
	static bool WorlPointOnScreen(glm::dvec3 worldPos, Camera& camera);
	static void DrawLine(glm::dvec3 from, glm::dvec3 to, glm::vec4 Color, Shader* shader);
	static bool Button(Canvas* canvas, Rect rect, const char* name, Style style);
	static bool Button(Canvas* canvas, Rect rect, const char *name, const char *style);
	static bool Button(Canvas* canvas, Rect rect, const char *name);
	static void Label(Canvas* canvas, Rect rec, const char *text, Style style);
	static void Label(Canvas* canvas, Rect rec, const char *text, const char *style);
	static void Label(Canvas* canvas, Rect rec, const char *text);
	static Rect BeginWindow(Canvas* canvas,int id, Rect rec, const char *title,Style style);
	static Rect BeginWindow(Canvas* canvas,int id, Rect rec, const char *title, const char *style);
	static Rect BeginWindow(Canvas* canvas,int id, Rect rec, const char *title);
	static void EndWindow(Canvas* canvas);

private:
	static int m_activeWindowID;
	static bool m_windowDragg;
};
#endif
