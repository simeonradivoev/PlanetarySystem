#include "Scene.h"
#include "Time.h"
Scene* Scene::CurrentScene;

Scene::Scene()
{
}

void Scene::Bind(){
	CurrentScene = this;
}

void Scene::AddGameObject(GameObject* obj){
	Scene::CurrentScene->Objects.push_back(obj);
}

void Scene::Draw(Camera& cam,Display& display){
	for (std::list<GameObject*>::iterator gameObject = Objects.begin(); gameObject != Objects.end(); gameObject++){
		(*gameObject)->Draw(cam, display);
	}
}

void Scene::Create(){

}

void Scene::Update(){
	
}

void Scene::FixedUpdate(){

}

Scene::~Scene()
{
	
}
