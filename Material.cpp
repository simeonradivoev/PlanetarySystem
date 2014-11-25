#include "Material.h"


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
	}

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int index = 0;
	for (std::list<Texture*>::iterator i = textures.begin(); i != textures.end(); i++){
		(*i)->Bind(index++);
	}
}

void Material::Unbind(){
	for (std::list<Texture*>::iterator i = textures.begin(); i != textures.end(); i++){
		(*i)->Unbind();
	}
}

void Material::Update(const Transform& transform, const Camera& cam,Display& display){
	if (shader != NULL){
		shader->Update(transform, cam, display);
	}
}


Material::~Material()
{
	
}
