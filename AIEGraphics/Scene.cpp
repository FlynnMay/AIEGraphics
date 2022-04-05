#include "Scene.h"
#include "Instance.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "ParticleEmitter.h"
#include <Gizmos.h>
#include <imgui.h>
#include <string>

Scene::Scene(Camera** _cameras, int _cameraCount, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight) :
	m_cameras(_cameras), m_cameraCount(_cameraCount), m_cameraIndex(0), m_windowSize(_windowSize), m_globalDirlight(_light), m_ambientLight(_ambientLight)
{
	AddPointLights(Light({ 0,0,0 }, { 1,1,1 }, 1));
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete (*it);
	}
}

void Scene::AddInstance(Instance* _instance)
{
	m_instances.push_back(_instance);
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < GetLightCount(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;
		aie::Gizmos::addSphere(m_pointLights[i].direction, 0.25f, 8, 8, glm::vec4(m_pointLights[i].colour, 1.0f));
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::Update(float _dt)
{
	m_sceneRunTime += _dt;

	// Camera GUI
	ImGui::Begin("Camera Settings");
	ImGui::DragInt("Active Camera", &m_cameraIndex, 0.1f, 0, GetCameraCount() - 1);

	bool debugMode = GetCamera()->GetDebugMode();
	ImGui::Checkbox("Debug Camera", &debugMode);
	
	FlyCamera* flyCam = dynamic_cast<FlyCamera*>(GetCamera());
	if (flyCam != nullptr)
	{
		float speed = flyCam->GetSpeed();
		ImGui::DragFloat("FlyCam Speed", &speed, 1, 0, 10.0f);
		flyCam->SetSpeed(speed);
	}

	ImGui::End();
	GetCamera()->SetDebugMode(debugMode);
	GetCamera()->Update(_dt);

	// Light GUI
	ImGui::Begin("Light Settings");

	ImGui::DragFloat3("Sunlight Direction", &GetGlobalLight().direction[0], 0.1f, -0.1f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &GetGlobalLight().colour[0], 0.1f, 0.0f, 2.0f);
	for (int i = 0; i < GetLightCount(); i++)
	{
		std::string iString = std::to_string(i + 1);
		if (ImGui::CollapsingHeader(iString.append(std::string(": Light")).c_str()))
		{
			std::string dirString = "Direction##";
			std::string colString = "Colour##";

			ImGui::DragFloat3(dirString.append(iString).c_str(), &GetPointLights()[i].direction[0], 0.1f);
			ImGui::ColorEdit3(colString.append(iString).c_str(), &GetPointLights()[i].colour[0]);
		}
	}
	ImGui::End();

	// Particle GUI
	ImGui::Begin("Particle Settings");
	int i = 0;
	for (auto it = m_particles.begin(); it != m_particles.end(); it++)
	{
		ParticleEmitter* particle = *it;

		std::string iString = std::to_string(i + 1);
		if (ImGui::CollapsingHeader(iString.append(std::string(": Particle")).c_str()))
		{
			float emitRate = particle->GetEmitRate();
			ImGui::DragFloat(std::string("Emission Rate##").append(iString).c_str(), &emitRate, 0.1f, 0.0001f);
			particle->SetEmitRate(emitRate);
		}		
		i++;
	}

	ImGui::End();

	m_pointLights[0].direction = glm::vec3(glm::cos(m_sceneRunTime), m_pointLights[0].direction.y, glm::sin(m_sceneRunTime * 2) / 2) * 10.0f;
}

Camera* Scene::GetCamera()
{
	return m_cameras[m_cameraIndex];
}

Camera* Scene::GetCameraAt(int _index)
{
	return m_cameras[_index];
}

void Scene::SetCamera(Camera* _camera, int _index)
{
	m_cameras[_index] = _camera;
}

void Scene::AddCamera(Camera* _camera)
{
	m_cameras[++m_cameraCount] = _camera;
}

void Scene::AddParticle(ParticleEmitter* _particle)
{
	m_particles.push_back(_particle);
}
