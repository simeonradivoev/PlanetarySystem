#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <list>

class Display
{
public:
	Display(int width,int height,const std::string& title);
	void Update();
	bool IsClosed();
	void Clear(GLbitfield mask);
	inline void SetClearColor(glm::vec3 color){ m_backgroundColor = color; }
	inline glm::vec3 GetClearColor(){ return m_backgroundColor; }
	void Bind();
	GLFWwindow* GetWindow(){ return m_window; }
	static Display* GetCurrentDisplay(){ return Display::m_currentDisplay; }
	int GetWidth(){ return m_width; }
	int GetHeight(){ return m_height; }
	void SetSize(unsigned int width, unsigned int height){ m_width = width; m_height = height; }
	virtual ~Display();
	static glm::vec2& HVInput;
private:
	Display(const Display& other){}
	Display& operator=(const Display& other){}
	GLFWwindow* m_window;
	glm::vec3 m_backgroundColor;
	bool m_isClosed;
	int mouseVisible;
	static Display* m_currentDisplay;
	int m_width;
	int m_height;
};
#endif DISPLAY_H