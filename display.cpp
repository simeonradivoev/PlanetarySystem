#include <iostream>
#include "display.h"
#include <list>

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

glm::vec2& Display::HVInput = glm::vec2(0, 0);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}

static void mouse_callback(GLFWwindow* window, double x, double y)
{
	
}



Display::Display(int width,int height,const std::string& Title)
{
	if (!glfwInit())
		exit(EXIT_FAILURE);

	//error handling
	glfwSetErrorCallback(error_callback);

	//create a window trough SDL
	m_window = glfwCreateWindow(width, height, Title.c_str(), NULL,NULL);

	if (!m_window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}


	//key input handling
	glfwSetKeyCallback(m_window, key_callback);

	//OpenGL context
	glfwMakeContextCurrent(m_window);

	//hide and lock the cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	GLenum status = glewInit();

	if (status != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize " << std::endl;
	}

	m_isClosed = false;

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	//cull back faces
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//enable transperancy
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Display::~Display()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

bool Display::IsClosed()
{
	return m_isClosed;
}

void Display::Clear(float r, float g, float b){
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::DrawViewport(){
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glViewport(0, 0, width, height);
}

void Display::Update()
{
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}