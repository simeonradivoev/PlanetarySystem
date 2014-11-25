#pragma once
#include <string>
#include <GL\glew.h>
#include <list>
#include "GLFW\glfw3.h"
#include "glm\glm.hpp"
#include "FBORenderTexture.h"


class Display
{
public:
	Display(int width,int height,const std::string& title);
	void Update();
	bool IsClosed();
	void DrawViewport();
	void Clear(float r,float g,float b);
	GLFWwindow* GetWindow(){ return m_window; }
	virtual ~Display();
	static glm::vec2& HVInput;
private:
	Display(const Display& other){}
	Display& operator=(const Display& other){}
	GLFWwindow* m_window;
	bool m_isClosed;
	int mouseVisible;
};