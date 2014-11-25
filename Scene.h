#pragma once
#include "GameObject.h"
#include <list>

	class Scene
	{
	public:
		Scene();
		~Scene();
		void Bind();
		static Scene* CurrentScene;
		virtual void Draw(Camera& cam, Display& display);
		static void AddGameObject(GameObject* obj);
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Create();
	private:
		std::list<GameObject*> Objects;
	};
