#pragma once
#include<string>
#include "transform.h"
#include "camera.h"
#include <GL\glew.h>

class Shader
{
public:
	Shader(const std::string& fileName);
	void Bind();
	void Update(const Transform& transform,const Camera& camera,Display& display);
	static std::string LoadShader(const std::string& fileName);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);
	void SetColor(glm::vec4 color);
	GLuint GetProgram(){ return m_program; }
	virtual ~Shader();
private:
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	static const unsigned int NUM_SHADERS = 2;

	enum MyEnum
	{
		MVP_U,
		MMV_U,
		COLOR_U,
		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};

