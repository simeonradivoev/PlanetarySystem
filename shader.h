#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "transform.h"
#include "camera.h"
#include <GL\glew.h>
#include<string>
#include <map>

class Shader
{
public:
	Shader(const std::string& fileName);
	void Bind();
	void Update(Transform& transform,Camera& camera);
	void Reset();
	static std::string LoadShader(const std::string& fileName);
	static GLuint CreateShader(const std::string& text, GLenum shaderType);
	static void LoadShaderData(GLuint shader, const std::string& data);
	void SetColor(glm::vec4 color);
	void SetEmission(float amount);
	GLuint GetProgram(){ return m_program; }
	void AddUniform(const char *name);
	void AddAllUniforms(std::string shaderData);
	operator GLuint() const{ return m_program; }
	void Reload();

	static void LoadAllShaders();
	static void ReloadAllShaders();
	static void DeleteAllShaders();
	static bool HasShader(const char *name){ return m_shadersMap.find(name) == m_shadersMap.end(); }
	static Shader* FindShader(const char *name){ return m_shadersMap[name]; }
	void DeleteShader();
	virtual ~Shader();
	
private:
	static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	static const unsigned int NUM_SHADERS = 2;

	enum MyEnum
	{
		PROJECTION_MATRIX_U,
		VIEW_MATRIX_U,
		MODEL_MATRIX_U,
		COLOR_U,
		EMISSION_U,
		NUM_UNIFORMS
	};

	GLuint m_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
	std::string m_name;
	static std::map<const char*, Shader*> m_shadersMap;
	std::map<const char*, GLuint> m_uniformsMap;
};
#endif SHADER_H