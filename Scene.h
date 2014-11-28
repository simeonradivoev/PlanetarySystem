#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"
#include <list>

class LightPass;

class Scene
{
public:
	Scene();
	~Scene();
	void Bind();
	static Scene* CurrentScene;
	virtual void GeometryPass(Camera& cam);
	virtual void LightingPass(Camera& cam, LightPass* lightingPass);
	static void AddGameObject(GameObject* obj);
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Create();
private:
	std::list<GameObject*> Objects;
};
#endif //SCENE_H