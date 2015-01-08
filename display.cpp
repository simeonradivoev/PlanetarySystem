#include "display.h"
#include <iostream>
#include <list>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	if (Display::GetCurrentDisplay() != nullptr)
		Display::GetCurrentDisplay()->SetSize(width, height);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

Display* Display::m_currentDisplay;

Display::Display(int width, int height, const std::string& Title)
: m_width(width), m_height(height)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//error handling
	glfwSetErrorCallback(error_callback);

	//create a window trough SDL
	m_window = glfwCreateWindow(width, height, Title.c_str(), NULL, NULL);

	if (!m_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//OpenGL context
	glfwMakeContextCurrent(m_window);

	//hide and lock the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//set automatic GL viewport ajustment
	glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize " << std::endl;
	}

	m_isClosed = false;
}

void Display::Bind(){
	m_currentDisplay = this;
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::Clear(GLbitfield mask)
{
	glClearColor(m_backgroundColor.x, m_backgroundColor.y, m_backgroundColor.z, 1.0f);
	glClear(mask);
}

void Display::Update()
{
	glfwSwapBuffers(m_window);
}

Display::~Display()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}