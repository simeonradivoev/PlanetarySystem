#pragma once

#ifndef GUI_CANVAS_H
#define GUI_CANVAS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include "nanovg.h"
#include "gui.h"

class NVGcontext;

class Canvas
{
public:
	struct Event
	{

	};
	Canvas(GLFWwindow* window, int width, int height);
	NVGcontext* GetContext(){ return m_context; }
	void Begin();
	void End();
	GUI::Style& GetCurrentStyle(){ return m_currentStyle; }
	GUI::Style GetStyle(const char *name){ return m_styles[name]; }
	void AddStyle(char *name, GUI::Style style){ m_styles[name] = style; }
	int GetImage(const char *name){ return m_images[name]; }
	void AddImage(const char *name, int image){ m_images[name] = image; }
	int LoadImage(const char *name, char *filename){ int handle = nvgCreateImage(m_context, filename, 0); m_images[name] = handle; return handle; }
	~Canvas();
	int earthIconHandle;
private:
	void CreateDefaultStyles();
	int m_width;
	int m_height;
	NVGcontext* m_context;
	GLFWwindow* m_window;
	GUI::Style m_currentStyle;
	std::map<const char *, GUI::Style> m_styles;
	std::map<const char *, int> m_images;
};
#endif

