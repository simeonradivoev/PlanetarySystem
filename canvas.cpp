#include "canvas.h"
#include "Core\Input.h"
#define NANOVG_GLES3_IMPLEMENTATION
#include "nanovg.h"
#include "nanovg_gl.h"

Canvas::Canvas(GLFWwindow* window, int width, int height)
:m_width(width), m_height(height), m_window(window)
{
	m_context = nvgCreateGLES3(NVG_ANTIALIAS);
	nvgCreateFont(m_context,"roboto","./res/fonts/Roboto-Regular.ttf");
	CreateDefaultStyles();
}

void Canvas::Begin(){ nvgBeginFrame(m_context, m_width, m_height, m_width / m_height); }
void Canvas::End(){ nvgEndFrame(m_context); }

void Canvas::CreateDefaultStyles()
{
	GUI::Style createPlanetButton = GUI::Style();
	createPlanetButton.imageHandler = LoadImage("earthIcon","./res/icons/earth.png");
	createPlanetButton.SetMargin(8);
	createPlanetButton.imageAlpha = 0.5;
	createPlanetButton.normalFillColor.a = 0.3;
	createPlanetButton.overFillColor = nvgRGBA(66, 209, 0, 30);
	createPlanetButton.pressFillColor = nvgRGBA(66, 209, 0, 100);

	GUI::Style createSunButton = GUI::Style(createPlanetButton);
	createSunButton.imageHandler = LoadImage("sunIcon", "./res/icons/sun.png");
	createSunButton.overFillColor = nvgRGBA(255, 160, 0, 30);
	createSunButton.pressFillColor = nvgRGBA(255, 160,0,100);

	AddStyle("createPlanetButton", createPlanetButton);
	AddStyle("createSunButton", createSunButton);
}


Canvas::~Canvas()
{
	nvgDeleteGLES3(m_context);
}
