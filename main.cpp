#include <iostream>
#include "display.h"
#include <string>
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "GameObject.h"
#include "FlyCamera.h"
#include "camera.h"
#include "PlanetSystem.h"
#include "Input.h"
#include "Time.h"
#include "FBORenderTexture.h"
#include "DeferredRendering.h"
#include <GL\glew.h>

#define WIDTH 1280
#define HEIGH 720

int main(int argc, char *argv[]){
	//create the display
	Display display(WIDTH, HEIGH, "Hello World");
	Input input(display);
	Time time = Time();

	//Create the main scene
	PlanetSystem mainScene = PlanetSystem();
	mainScene.Bind();
	mainScene.Create();

	//create the camera
	FlyCamera camera( 90, WIDTH / HEIGH, 0.01f, 1000.0f);
	camera.transform.position = glm::dvec3(0, 0, -3);
	Camera light(90,WIDTH/HEIGH,4,100);
	light.transform.position = glm::dvec3(0, -6.5, -11.0);

	FBORenderTexture m_multipleRenderTarget(WIDTH, HEIGH);
	DepthRenderTexture m_depthTexture(WIDTH, HEIGH);
	DeferredRendering m_deferredRendering(WIDTH, HEIGH, &m_multipleRenderTarget, &m_depthTexture);
	

	while (!glfwWindowShouldClose(display.GetWindow()))
	{
		display.DrawViewport();
		display.Clear(0.0f, 0.15f, 0.3f);
		input.ManageInput(display);

		//manage camera input
		camera.ManageInput(display);

		time.Update(mainScene);
		glFrontFace(GL_CW);

		m_deferredRendering.startRenderToShadowMap();
		mainScene.Draw(light, display);
		m_deferredRendering.endRenderToShadowMap();
		

		glFrontFace(GL_CCW);
		m_deferredRendering.startRenderToFBO();
		//draw the scene and all the gameobjects to save to Buffers
		mainScene.Draw(camera,display);
		m_deferredRendering.endRenderToFBO();
		
		m_deferredRendering.setLightMatix(camera.GetTransform().GetModel(),light.GetViewProjection(display), light.GetTransform().GetModel());
		//deffered rendering
		m_deferredRendering.render(camera);

		display.Update();
	}

	return 0;
}