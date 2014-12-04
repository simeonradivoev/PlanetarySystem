#pragma once
#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#define SPHERE_PRIMITIVE_SEGMENTS 5

#include <string>
#include "transform.h"
#include "camera.h"
#include "mesh.h"

class Material;
class Shader;

class GameObject
{
public:
	GameObject(const std::string Name);
	GameObject(const std::string Name,Mesh* mesh, Material* mat);
	~GameObject();
	void Draw(Camera& camera);
	void Draw(Camera& camera, Shader* shader);
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
	static GameObject* CreatePlane(const double size, Material* mat);
	static Mesh* GetPlaneMesh(){ if (m_planeMesh == nullptr) m_planeMesh = Mesh::Plane(1); return m_planeMesh; }
	static Mesh* GetSphereMesh(){ if (m_sphereMesh == nullptr) m_sphereMesh = Mesh::Sphere(1, SPHERE_PRIMITIVE_SEGMENTS); return m_sphereMesh; }
	static void DrawSphere(Transform transform, Material* material, Camera& camera);
	static void DrawSphere(Transform transform, Shader* material,Camera& camera);
	static void DrawPlane(Transform transform, Material* material,Camera& camera);
	static void DrawPlane(Transform transform, Shader* shader,Camera& camera);
private:
	Mesh* m_mesh;
	Material* mat;
	Transform _transform;
	std::string Name;
	static Mesh* m_planeMesh;
	static Mesh* m_sphereMesh;
};

#endif //GAMEOBJECT_H
