#include "Material.h"
#include "shader.h"
#include "texture.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}

void Material::AddTexture(Texture* tex){
	textures.push_back(tex);
}

void Material::Bind(){
	if (shader != nullptr){
		shader->Bind();
		shader->SetColor(m_color);
		shader->SetEmission(m_emission);
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int index = 0;
	for (std::list<Texture*>::iterator i = textures.begin(); i != textures.end(); i++){
		(*i)->Bind(index++);
	}
}

void Material::Unbind(){
	unsigned int index = 0;
	for (std::list<Texture*>::iterator i = textures.begin(); i != textures.end(); i++){
		(*i)->Unbind(index++);
	}
}

void Material::Update(Transform& transform, Camera& cam){
	if (shader != NULL){
		shader->Update(transform, cam);
		shader->SetColor(m_color);
		shader->SetEmission(m_emission);
	}
}


Material::~Material()
{
	
}
