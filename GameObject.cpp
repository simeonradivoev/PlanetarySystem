#include "GameObject.h"
#include "camera.h"
#include "Scene.h"

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

void GameObject::Draw(const Camera& camera,Display& display){
		mat->Bind();

	if (m_mesh != NULL){
		mat->Update(transform, camera, display);
		m_mesh->Draw();
	}
}

GameObject* GameObject::CreateSphere(const double radius, const int segments,Material* mat){
	
	double angle = 0;
	const unsigned int VertexCount = (180 / segments) * (360 / segments) * 4;
	const unsigned int indexCount = (VertexCount / 2) * 3;
	VERTICES* vertex = new VERTICES[VertexCount];
	unsigned int* indices = new unsigned int[indexCount];
	//current vertex
	int n;
	//current index
	int i;
	//loop controls
	double a,b;
	double H = 0,K = 0,Z = 0;

	n = 0;
	i = 0;

	for (b = 0; b <= 180 - segments; b += segments){
		for (a = 0; a <= 360 - segments; a += segments){
			vertex[n].GetPos()->x = radius * sin(glm::radians(a)) * sin(glm::radians(b)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a)) * sin(glm::radians(b)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b)) - Z;

			vertex[n].GetUV()->y = (b) / 180.0;
			vertex[n].GetUV()->x = (a) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//b + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a)) * sin(glm::radians(b + segments)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a)) * sin(glm::radians(b + segments)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b + segments)) - Z;

			vertex[n].GetUV()->y = (b + segments) / 180.0;
			vertex[n].GetUV()->x = (a) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//a + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a + segments)) * sin(glm::radians(b)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a + segments)) * sin(glm::radians(b)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b)) - Z;

			vertex[n].GetUV()->y = (b) / 180.0;
			vertex[n].GetUV()->x = (a + segments) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//a + b + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a + segments)) * sin(glm::radians(b + segments)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a + segments)) * sin(glm::radians(b + segments)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b + segments)) - Z;

			vertex[n].GetUV()->y = (b + segments) / 180.0;
			vertex[n].GetUV()->x = (a + segments) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			indices[i++] = n - 4;
			indices[i++] = n - 3;
			indices[i++] = n - 1;

			indices[i++] = n - 4;
			indices[i++] = n - 1;
			indices[i++] = n - 2;

			
		}
	}
	return new GameObject("Sphere", new Mesh(vertex, VertexCount, indices, indexCount), mat);
}

GameObject::~GameObject()
{
}
