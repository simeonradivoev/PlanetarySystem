#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "camera.h"
#include "transform.h"
#include <list>
#include <glm\glm.hpp>

class Shader;
class Texture;

class Material
{
public:
	Material(Shader* shader);
	~Material();
	void AddTexture(Texture* tex);
	void Bind();
	void Unbind();
	void Update(Transform& transform, Camera& cam);
	void SetColor(glm::vec4 mainColor){ m_color = mainColor; }
	glm::vec4& GetColor(){ return m_color; }
	void SetEmission(float emission){ m_emission = emission; }
	Shader* GetShader(){ return shader; }
private:
	Shader* shader;
	std::list<Texture*> textures;
	glm::vec4 m_color = glm::vec4(1,1,1,1);
	float m_emission = 0;
};
#endif //MATERIAL_H