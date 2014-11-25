#pragma once
#include "mesh.h"
#include "Material.h"
#include "transform.h"
#include <string>

class GameObject
{
public:
	GameObject(const std::string Name);
	GameObject(const std::string Name,Mesh* mesh, Material* mat);
	~GameObject();
	void Draw(const Camera& camera,Display& display);
	_declspec(property(get = GetTransform, put = SetTransform)) Transform& transform;
	_declspec(property(get = GetMesh, put = SetMesh)) Mesh* mesh;
	_declspec(property(get = GetMaterial, put = SetMaterial)) Material* material;
	_declspec(property(get = GetName, put = SetName)) std::string name;
	std::string GetName(){ return Name; }
	void SetName(std::string Name){ GameObject::Name = Name; }
	Transform& GetTransform(){ return _transform; }
	void SetTransform(Transform& transform){ _transform = transform; }
	Mesh* GetMesh(){ return m_mesh; }
	void SetMesh(Mesh* mesh){ m_mesh = mesh; }
	Material* GetMaterial(){ return mat; }
	void SetMaterial(Material* material){ mat = material; }
	static GameObject* CreateSphere(const double radius, const int segments, Material* mat);
private:
	Mesh* m_mesh;
	Material* mat;
	Transform _transform;
	std::string Name;
};

