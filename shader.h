#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "transform.h"
#include "camera.h"
#include <GL\glew.h>
#include<string>

class Shader
{
public:
	Shader(const std::string& fileName);
	void Bind();
	void Update(Transform& transform,Camera& camera);
	void Reset();
	static std::string LoadShader(const std::string& fileName);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);
	void SetColor(glm::vec4 color);
	GLuint GetProgram(){ return m_program; }
	virtual ~Shader();
	operator GLuint() const{ return m_program; }
private:
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	static const unsigned int NUM_SHADERS = 2;

	enum MyEnum
	{
		VIEW_PROJECTION_MATRIX_U,
		MODEL_MATRIX_U,
		COLOR_U,
		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};
#endif SHADER_H