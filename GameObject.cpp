#include "GameObject.h"
#include "Scene.h"
#include "Material.h"
#include "shader.h"
#include "mesh.h"

Mesh* GameObject::m_sphereMesh = nullptr;
Mesh* GameObject::m_planeMesh = nullptr;

GameObject::GameObject(const std::string Name)
{
	this->Name = Name;
	Scene::CurrentScene->AddGameObject(this);
}

GameObject::GameObject(const std::string Name, Mesh* mesh, Material* mat)
{
	this->Name = Name;
	this->m_mesh = mesh;
	this->mat = mat;
	Scene::CurrentScene->AddGameObject(this);
}

void GameObject::Draw(Camera& camera){
		mat->Bind();

	if (m_mesh != NULL){
		mat->Update(transform, camera);
		m_mesh->Draw();
	}
}

void GameObject::Draw(Camera& camera, Shader* shader){
	shader->Update(_transform,camera);
	m_mesh->Draw();
}

GameObject* GameObject::CreateSphere(const double radius, const int segments,Material* mat)
{
	return new GameObject("Sphere", Mesh::Sphere(radius, segments), mat);
}

GameObject* GameObject::CreatePlane(const double size,Material* mat)
{
	return new GameObject("Plane", Mesh::Plane(size), mat);
}

void GameObject::DrawSphere(Transform transform, Material* material, Camera& camera)
{
	material->Bind();
	material->Update(transform,camera);
	GetSphereMesh()->Draw();
}

void GameObject::DrawSphere(Transform transform, Shader* shader, Camera& camera)
{
	shader->Bind();
	shader->Update(transform, camera);
	GetSphereMesh()->Draw();
}

void GameObject::DrawPlane(Transform transform,Material* material,Camera& camera){
	material->Bind();
	material->Update(transform, camera);
	GetPlaneMesh()->Draw();
}

void GameObject::DrawPlane(Transform transform, Shader* shader, Camera& camera){
	shader->Bind();
	shader->Update(transform, camera);
	GetPlaneMesh()->Draw();
}

GameObject::~GameObject()
{
}
