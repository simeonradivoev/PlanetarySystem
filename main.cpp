#include "display.h"
#include <string>
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "GameObject.h"
#include "FlyCamera.h"
#include "camera.h"
#include "Core/Input.h"
#include "Core/Time.h"
#include "Gbuffer.h"
#include "DeferredRendering.h"
#include "renderTexture.h"
#include "canvas.h"
#include "PlanetSystem.h"
#include "lightPass.h"
#include "gui.h"

#define WIDTH 1280
#define HEIGH 720

Gbuffer* m_Gbuffer;
LightPass* m_lighPass;
DeferredRendering* m_deferredRendering;

void GeometryPass(Scene& scene,Camera& camera,Display& display){
	m_Gbuffer->start();
	//draw the scene and all the gameobjects to save to Buffers
	scene.GeometryPass(camera);
	m_Gbuffer->stop();
}

void LightingPass(Scene& scene, Camera& camera)
{
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);

	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Gbuffer->getFrameBuffer());

	glReadBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, WIDTH, HEIGH, 0, 0, WIDTH, HEIGH, GL_COLOR_BUFFER_BIT, GL_LINEAR);*/

	glEnable(GL_STENCIL_TEST);
	m_lighPass->StencilTest();
	scene.LightingPass(camera, m_lighPass);

	m_lighPass->Start(camera);
	scene.LightingPass(camera,m_lighPass);
	m_lighPass->End();
    glDisable(GL_STENCIL_TEST);

	//m_deferredRendering->render(camera);
}

void FinalPass(Camera& camera)
{
	m_Gbuffer->BindForFinalPass();
	m_deferredRendering->SetAmbientLighting(glm::vec3(0, 0.15, 0.3));
	m_deferredRendering->render(camera);
}

void GUITest(Canvas* canvas, Display& display,Scene& scene,Camera& camera)
{
	GUI::Label(canvas, GUI::Rect(16, 16, 128, 64), std::to_string(Time::GetFPS()).c_str());	//fps counter
	scene.GUI(canvas, camera);
}

int main(int argc, char *argv[]){
	//create the display
	Display display(WIDTH, HEIGH, "Hello World");
	display.Bind();
	Input input(display);
	Time time = Time();
	Shader::LoadAllShaders();
	Canvas* canvas = new Canvas(display.GetWindow(),WIDTH, HEIGH);

	m_Gbuffer = new Gbuffer(WIDTH, HEIGH);
	m_lighPass = new LightPass(m_Gbuffer);
	m_deferredRendering = new DeferredRendering(WIDTH, HEIGH, m_Gbuffer);

	//Create the main scene
	PlanetSystem mainScene = PlanetSystem();
	mainScene.Bind();
	mainScene.Create();

	//create the camera
	FlyCamera camera( 75, WIDTH / HEIGH, 0.01f, 1000.0f);
	camera.Bind();
	camera.transform.position = glm::dvec3(0, 6, -6);
	Camera light(75,WIDTH/HEIGH,4,100);
	light.transform.position = glm::dvec3(0, 0, -20.0);

	while (!glfwWindowShouldClose(display.GetWindow()))
	{
		//updates time
		time.Update(mainScene);

		display.Clear(0, 0.15, 0.3);
		input.ManageInput(display);
		camera.OnRender(display);
		
		m_Gbuffer->StartFrame();
		m_Gbuffer->StartFrame();
		//render all geometry to FBO
		GeometryPass(mainScene,camera,display);
		//StencilTest();
		//calculate lighting
		LightingPass(mainScene, camera);
		//dump the GBuffer on screen
		FinalPass(camera);

		canvas->Begin();
		GUITest(canvas, display, mainScene, camera);
		canvas->End();


		//update the display and swap the buffers
		display.Update();
		input.PollEvents();
	}

	return 0;
}