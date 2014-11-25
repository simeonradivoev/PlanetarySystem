#pragma once
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "transform.h"
#include <list>

class Material
{
public:
	Material(Shader* shader);
	~Material();
	void AddTexture(Texture* tex);
	void Bind();
	void Unbind();
	void Update(const Transform& transform, const Camera& cam,Display& display);
	void SetColor(glm::vec4 mainColor){ m_color = mainColor; }
private:
	Shader* shader;
	std::list<Texture*> textures;
	glm::vec4 m_color = glm::vec4(1,1,1,1);
};

