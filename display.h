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
	void Clear(float r,float g,float b);
	void Bind();
	GLFWwindow* GetWindow(){ return m_window; }
	static Display* GetCurrentDisplay(){ return Display::m_currentDisplay; }
	virtual ~Display();
	static glm::vec2& HVInput;
private:
	Display(const Display& other){}
	Display& operator=(const Display& other){}
	GLFWwindow* m_window;
	bool m_isClosed;
	int mouseVisible;
	static Display* m_currentDisplay;
};
#endif DISPLAY_H