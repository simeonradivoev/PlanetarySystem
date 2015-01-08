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

void Canvas::Begin()
{
	
	
	m_controlCounter = 0;
	BeginLayout(0, 0,m_width,m_height);
	nvgBeginFrame(m_context, m_width, m_height, (float)m_width / (float)m_height);
}
void Canvas::End()
{ 
	if (Input::GetCurrentEvent().mouseAction == GLFW_PRESS && Input::GetCurrentEvent().mouseButton == GLFW_MOUSE_BUTTON_1 && !m_currentEvent.Events[CE_ACTIVE_CONTROL])
	{
		m_activeControl = 0;
	}

	EndLayout();
	while (!m_currentLayout.empty())
	{
		m_currentLayout.pop();
	}
	nvgEndFrame(m_context); 

	m_lastEvent = Event(m_currentEvent);
	m_currentEvent = Event();
}

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

	GUI::Style label = GUI::Style(createPlanetButton);
	label.textAllignment = NVG_ALIGN_LEFT | NVG_ALIGN_TOP;

	GUI::Style window = GUI::Style(createPlanetButton);
	window.normalFillColor = nvgRGBA(5, 10, 20, 220);
	window.pressFillColor = nvgRGBA(5, 10, 20, 220);
	window.topMargin = 20;

	GUI::Style textField = GUI::Style(createPlanetButton);
	textField.textAllignment = NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE;

	GUI::Style slider = GUI::Style(createPlanetButton);

	AddStyle("createPlanetButton", createPlanetButton);
	AddStyle("createSunButton", createSunButton);
	AddStyle("label", label);
	AddStyle("window", window);
	AddStyle("textField", textField);
	AddStyle("slider", slider);
}


Canvas::~Canvas()
{
	nvgDeleteGLES3(m_context);
}
