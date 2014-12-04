#include "shader.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\transform.hpp>

#define SHADER_FOLDER "./res/shaders/"
const std::string INCLUDE_DIRECTIVE = "#include";

std::map<const char*, Shader*> Shader::m_shadersMap;

Shader::Shader(const std::string& fileName)
{
	m_program = glCreateProgram();
	std::string vShaderData = LoadShader(fileName + ".vs");
	std::string fShaderData = LoadShader(fileName + ".fs");
	m_shaders[0] = CreateShader(vShaderData, GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(fShaderData, GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glAttachShader(m_program, m_shaders[i]);
	}

	AddAllUniforms(vShaderData);
	AddAllUniforms(fShaderData);

	glBindAttribLocation(m_program,0,"position");
	glBindAttribLocation(m_program, 1, "uv");
	glBindAttribLocation(m_program, 2, "normal");

	glLinkProgram(m_program);
	CheckShaderError(m_program,GL_LINK_STATUS,true,"Error: Program linking failed!");

	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid!");

	m_uniforms[PROJECTION_MATRIX_U] = glGetUniformLocation(m_program, "ProjectionMatrix");
	m_uniforms[VIEW_MATRIX_U] = glGetUniformLocation(m_program, "ViewMatrix");
	m_uniforms[MODEL_MATRIX_U] = glGetUniformLocation(m_program, "ModelMatrix");
	m_uniforms[COLOR_U] = glGetUniformLocation(m_program, "color");
	m_uniforms[EMISSION_U] = glGetUniformLocation(m_program, "emmision");
}


Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		glDetachShader(m_program,m_shaders[i]);
		glDeleteShader(m_shaders[i]);
	}

	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	glUseProgram(m_program);
}

void Shader::Update(Transform& transform,Camera& camera)
{
	glm::dmat4 projection = camera.GetProjection();
	glm::dmat4 view = camera.GetViewMatrix();
	glm::dmat4 model = transform.GetModel();

	glUniformMatrix4fv(m_uniforms[PROJECTION_MATRIX_U], 1, GL_FALSE, &((glm::mat4)projection)[0][0]);
	glUniformMatrix4fv(m_uniforms[VIEW_MATRIX_U], 1, GL_FALSE, &((glm::mat4)view)[0][0]);
	glUniformMatrix4fv(m_uniforms[MODEL_MATRIX_U], 1, GL_FALSE, &((glm::mat4)model)[0][0]);
}

void Shader::Reset(){
	glUniformMatrix4fv(m_uniforms[PROJECTION_MATRIX_U], 1, GL_FALSE, &(glm::mat4()[0][0]));
	glUniformMatrix4fv(m_uniforms[VIEW_MATRIX_U], 1, GL_FALSE, &(glm::mat4()[0][0]));
	glUniformMatrix4fv(m_uniforms[MODEL_MATRIX_U], 1, GL_FALSE, &(glm::mat4()[0][0]));
}

void Shader::SetColor(glm::vec4 color){
	glUniform4fv(m_uniforms[COLOR_U], 1, glm::value_ptr(color));
}

void Shader::SetEmission(float emission){
	glUniform1f(m_uniforms[EMISSION_U],emission);
}


GLuint Shader::CreateShader(const std::string& text, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	if (shader == 0)
		std::cerr << "Error: Shader creation failed" << std::endl;

	const GLchar* shaderSourceStrings[1];
	GLint shaderSourceStringLengths[1];

	shaderSourceStrings[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	glShaderSource(shader, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error: Shader compilation failed!");

	return shader;
}

std::string Shader::LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((SHADER_FOLDER + fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);

			if (line.compare(0, INCLUDE_DIRECTIVE.length(), INCLUDE_DIRECTIVE) == 0)
			{
				int fileBegin;
				int fileEnd;

				fileBegin = line.find('\"') +1;
				fileEnd = line.find('\"', fileBegin);

				output.append(LoadShader(line.substr(fileBegin, fileEnd - fileBegin)));
			}
			else
			{
				output.append(line + "\n");
			}
		}
	}
	else
	{
		std::cerr << "Unable to load shader: " << fileName << std::endl;
	}

	return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}


void Shader::AddUniform(const char *name)
{
	m_uniformsMap[name] = glGetUniformLocation(m_program,name);
}

std::string RemoveSpaces(std::string string, int pos,int& end)
{
	int begin = pos;
	while (string[begin] == ' ' && begin < string.length())
	{
		begin++;
	}

	end = begin;
	while (string[end] != ' ' && end < string.length())
	{
		end++;
	}

	return string.substr(begin, end - begin);
}

void Shader::AddAllUniforms(std::string shaderData)
{
	std::string uniformKeyName = "uniform";
	int uniformStartLocation = shaderData.find(uniformKeyName);

	while (uniformStartLocation != std::string::npos)
	{
		int begin = uniformStartLocation + uniformKeyName.length() + 1;
		int end = shaderData.find(';', begin);
		while (shaderData[begin] == ' ')
		{
			begin++;
		}

		std::string uniformLine = shaderData.substr(begin, end - begin);
		std::string uniformType = RemoveSpaces(uniformLine, 0, end);
		std::string uniformName = RemoveSpaces(uniformLine, end, end);
		std::cout << "[" + uniformType + "](" + uniformName + ")" << std::endl;

		uniformStartLocation = shaderData.find(uniformKeyName, uniformStartLocation + uniformKeyName.length());
	}
}

void Shader::LoadAllShaders()
{
	m_shadersMap["deferredRendering"] = new Shader("deferredRendering");
	m_shadersMap["geometryPass"] = new Shader("geometryPass");
	m_shadersMap["gridShader"] = new Shader("gridShader");
	m_shadersMap["lightPass"] = new Shader("lightPass");
	m_shadersMap["orbitShader"] = new Shader("orbitShader");
	m_shadersMap["glowShader"] = new Shader("glowShader");
}

