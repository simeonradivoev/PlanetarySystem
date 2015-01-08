#include "Planet.h"
#include "Core/Time.h"
#include "Material.h"
#include "shader.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <glm\gtc\type_ptr.hpp>

#define MAX_ORBIT_LENGTH 512
#define ORBIT_SAMPLE_TIME 0.1

Planet::Planet(std::string name, double mass, glm::dvec3 velocity, double angularVelocity, double radius,double lightIntencity,Shader* shader,Texture* texture)
:m_name(name), m_mass(mass), m_velocity(velocity), m_angularVelocity(angularVelocity), m_radius(radius), m_lightIntencity(lightIntencity)
{
	if (name.empty())
		m_name = GetRandomName();

	Material* mat = new Material(shader);
	mat->AddTexture(texture);
	mat->SetEmission(lightIntencity);
	SetObject(new GameObject(name, GameObject::GetSphereMesh(), mat));
	GetObject()->GetTransform().scale = glm::dvec3(radius);
	m_orbitColor = glm::dvec3((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX);

	if (lightIntencity > 0)
	{
		m_light = new Light(glm::vec3(1), lightIntencity, 0, 1);
		m_light->SetAmbientIntencity(1);
	}
}

void Planet::DrawTail(Material* material,Camera& camera){
	material->SetColor(glm::vec4(m_orbitColor, 0.6));
	material->Bind();
	material->Update(Transform(), camera);
	

	glBegin(GL_LINE_STRIP);
	for (std::list<glm::dvec3>::iterator node = m_orbit.begin(); node != m_orbit.end(); node++)
	{
		glVertex3d(node->x,node->y,node->z);
	}
	glEnd();
}

void Planet::CalculateTail()
{
	m_orbitSampleTimer += Time::GetDeltaTime(TIME_DELTA_RAW) * Time::GetTimeScale();

	if (m_orbitSampleTimer > ORBIT_SAMPLE_TIME)
	{
		SaveOrbitPoint();
		m_orbitCounter++;

		while (m_orbitCounter > MAX_ORBIT_LENGTH)
		{
			m_orbit.pop_front();
			m_orbitCounter--;
		}

		m_orbitSampleTimer = 0;
	}
}

void Planet::SaveOrbitPoint()
{
	m_orbit.push_back(glm::dvec3(GetObject()->transform.position));
}

void Planet::ClearTail()
{
	m_orbit.clear();
}

void Planet::DrawPlanet(Camera& camera){
	if (m_object != nullptr){
		glCullFace(GL_BACK);
		m_object->Draw(camera);
	}
}

void Planet::DrawAtmosphere(Camera& camera)
{
	if (m_object != nullptr)
	{
		glCullFace(GL_FRONT);
		UpdateAtmosphereShaderUniforms(camera, Shader::FindShader("skyFromSpace"));
		//Shader::FindShader("skyFromSpace")->Bind();
		//Shader::FindShader("skyFromSpace")->Update(m_object->transform,camera);
		//GameObject::GetSphereMesh()->Draw();

		GameObject::DrawSphere(Transform(m_object->transform.position, glm::dquat(), glm::dvec3(m_radius + m_radius * 0.025)),Shader::FindShader("skyFromSpace"),camera);
		glCullFace(GL_BACK);
	}
}

void Planet::UpdateAtmosphereShaderUniforms(Camera& camera,Shader* shader){
		float m_Kr = 0.0025f;	// Rayleigh scattering constant
		float m_ESun = 20.0f;	// Sun brightness constant
		float m_Km = 0.0010f;	// Mie scattering constant
		float m_Kr4PI = m_Kr*4.0f*pi<float>();
		float m_Km4PI = m_Km*4.0f*pi<float>();
		float m_fRayleighScaleDepth = 0.25f;
		float m_g = -0.990f;		// The Mie phase asymmetry factor
		float m_outerRadius = m_radius + m_radius * 0.025;
		float m_innerRadius = m_radius;
		glm::dvec3 waveLength(0.650f, 0.570f, 0.475f);


		GLuint shaderProgram = shader->GetProgram();
		glUseProgram(shader->GetProgram());
		glUniform3fv(glGetUniformLocation(shaderProgram, "v3Center"), 1, glm::value_ptr((glm::vec3)m_object->transform.position));
		glUniform3fv(glGetUniformLocation(shaderProgram, "v3WorldCameraPos"), 1, glm::value_ptr((glm::vec3)camera.transform.position));
		glUniform3fv(glGetUniformLocation(shaderProgram, "v3WorldLightPos"), 1, glm::value_ptr(glm::vec3()));
		glUniform3f(glGetUniformLocation(shaderProgram, "v3InvWavelength"), 1.0f / powf(waveLength.x, 4.0f), 1.0f / powf(waveLength.y, 4.0f), 1.0f / powf(waveLength.z, 4.0f));
		glUniform1f(glGetUniformLocation(shaderProgram, "fInnerRadius"), m_innerRadius);
		glUniform1f(glGetUniformLocation(shaderProgram, "fInnerRadius2"), glm::pow(m_innerRadius, 2.0f));
		glUniform1f(glGetUniformLocation(shaderProgram, "fOuterRadius"), m_outerRadius);
		glUniform1f(glGetUniformLocation(shaderProgram, "fOuterRadius2"), glm::pow(m_outerRadius, 2.0f));
		glUniform1f(glGetUniformLocation(shaderProgram, "fKrESun"), m_Kr * m_ESun);
		glUniform1f(glGetUniformLocation(shaderProgram, "fKmESun"), m_Km * m_ESun);
		glUniform1f(glGetUniformLocation(shaderProgram, "fKr4PI"), m_Kr4PI);
		glUniform1f(glGetUniformLocation(shaderProgram, "fKm4PI"), m_Km4PI);
		glUniform1f(glGetUniformLocation(shaderProgram, "fScale"), 1.0f / (m_outerRadius - m_innerRadius));
		glUniform1f(glGetUniformLocation(shaderProgram, "fScaleDepth"), m_fRayleighScaleDepth);
		glUniform1f(glGetUniformLocation(shaderProgram, "fScaleOverScaleDepth"), 1.0f / (m_outerRadius - m_innerRadius) / m_fRayleighScaleDepth);
		glUniform1f(glGetUniformLocation(shaderProgram, "g"), m_g);
		glUniform1f(glGetUniformLocation(shaderProgram, "g2"), glm::pow(m_g, 2.0f));
		glUniform1f(glGetUniformLocation(shaderProgram, "fHdrExposure"), 0.05f);

}

void Planet::DrawSelection(Camera& camera,Shader* shader)
{
	GameObject::DrawPlane(Transform(GetObject()->GetTransform().position,glm::dquat(), GetObject()->transform.scale * 3.0), Shader::FindShader("glowShader"), camera);
}

void Planet::DrawLight(Camera& camera,LightPass* lightPass){
	if (m_light != nullptr){
		m_light->Draw(camera, lightPass);
	}
}

std::string Planet::GetRandomName()
{
	std::ifstream file;
	file.open("./res/planetNames.txt");

	std::string line;
	std::vector<std::string> names;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			names.push_back(line);
			std::cout << line << std::endl;
		}
	}

	return names[(int)(names.size() * ((double)rand() / RAND_MAX)) - 1];
}


Planet::~Planet()
{
	if (m_light != nullptr)
		delete m_light;

	if (m_object != nullptr)
	delete m_object;
}
