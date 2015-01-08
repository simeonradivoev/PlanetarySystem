#pragma once

#ifndef GUI_CANVAS_H
#define GUI_CANVAS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "nanovg.h"
#include "gui.h"
#include <stack>

class NVGcontext;

enum CanvasEvent
{
	CE_NONE,
	CE_ACTIVE_CONTROL,
	CE_PRESS,
	CE_NUM_EVENTS
};

class Canvas
{
public:
	struct Event
	{
	public:
		bool Events[CE_NUM_EVENTS];
		inline void AddEvent(CanvasEvent type){ Events[type] = true; }
	};
	struct Layout
	{
		float x;
		float y;
		float width;
		float height;

		Layout(float x, float y, float width, float height){ this->x = x; this->y = y; this->width = width; this->height = height; }
	};
	Canvas(GLFWwindow* window, int width, int height);
	NVGcontext* GetContext(){ return m_context; }
	void Begin();
	void End();
	inline GUI::Style& GetCurrentStyle(){ return m_currentStyle; }
	inline GUI::Style GetStyle(const char *name){ return m_styles[name]; }
	inline void BeginLayout(int x, int y, float width, float height){ m_currentLayout.push(Layout(x, y, width, height)); }
	inline void EndLayout(){ m_currentLayout.pop(); }
	inline Layout GetCurrentLayout(){ return m_currentLayout.top(); }
	inline void AddStyle(char *name, GUI::Style style){ m_styles[name] = style; }
	inline int GetImage(const char *name){ return m_images[name]; }
	inline void AddImage(const char *name, int image){ m_images[name] = image; }
	inline int LoadImage(const char *name, char *filename){ int handle = nvgCreateImage(m_context, filename, 0); m_images[name] = handle; return handle; }
	inline int GetActiveControl(){ return m_activeControl; }
	inline void SetActiveControl(int activeControl){ m_activeControl = activeControl; }
	inline Event& GetCurrentEvent(){ return m_currentEvent; }
	inline const Event GetLastEvent(){ return m_lastEvent; }
	inline int GetControl(){ return ++m_controlCounter; }
	inline unsigned int GetControlOffset(){ return m_controlOffset; }
	inline void SetControlOffset(unsigned int offset){ m_controlOffset = offset; }
	inline void IncreaseControlOffset(){ m_controlOffset++; }
	inline void DecreaseControlOffset(){ m_controlOffset--; }
	~Canvas();
	int earthIconHandle;
private:
	void CreateDefaultStyles();
	int m_width;
	int m_height;
	int m_controlCounter = 0;
	int m_activeControl = 0;
	unsigned int m_controlOffset = 0;
	NVGcontext* m_context;
	GLFWwindow* m_window;
	GUI::Style m_currentStyle;
	std::stack<Layout> m_currentLayout;
	Event m_currentEvent;
	Event m_lastEvent;
	std::map<const char *, GUI::Style> m_styles;
	std::map<const char *, int> m_images;
};
#endif

