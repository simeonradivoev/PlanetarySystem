#include "Scene.h"
#include "Time.h"
#include "lightPass.h"
#include "canvas.h"
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

void Scene::GeometryPass(Camera& cam){
	for (std::list<GameObject*>::iterator gameObject = Objects.begin(); gameObject != Objects.end(); gameObject++){
		(*gameObject)->Draw(cam);
	}
}

void Scene::LightingPass(Camera& cam,LightPass* pass){

}

void Scene::TransperentPass(Camera& cam)
{

}

void Scene::Create(){

}

void Scene::Update(){
	
}

void Scene::FixedUpdate(){

}

void Scene::GUI(Canvas* canvas, Camera& camera){

}

Scene::~Scene()
{
	
}
