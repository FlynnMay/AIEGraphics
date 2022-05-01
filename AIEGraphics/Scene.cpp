#include "Scene.h"
#include "Instance.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "ParticleEmitter.h"
#include "EasyShader.h"
#include "Shader.h"
#include <Gizmos.h>
#include <imgui.h>
#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>


Scene::Scene(Camera** _cameras, int _cameraCount, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight) :
	m_cameras(_cameras), m_cameraCount(_cameraCount), m_cameraIndex(0), m_windowSize(_windowSize), m_globalDirlight(_light), m_ambientLight(_ambientLight)
{
	AddPointLight(Light({ 0,0,0 }, { 1,1,1 }, 1));
	m_particleShader = new EasyShader();

	m_particleShader->SetOnShaderBind([=](auto s)
		{
			glm::mat4 projectionMatrix = GetCamera()->GetProjectionMatrix(GetWindowSize().x, GetWindowSize().y);
			glm::mat4 viewMatrix = GetCamera()->GetViewMatrix();

			glm::mat4 pTransform = {
				1.0f,0,0,0,
				0,1.0f,0,0,
				0,0,1.0f,0,
				0,0,0,1.0f
			};

			auto pvm = projectionMatrix * viewMatrix * pTransform;
			s->bindUniform("ProjectionViewModel", pvm);
		});
}

Scene::~Scene()
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		delete (*it);
	}
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
		instance->Draw();
	}

	for (auto it = m_particles.begin(); it != m_particles.end(); it++)
	{
		ParticleEmitter* particle = *it;
		m_particleShader->BindShader();
		particle->Draw();
	}

	for (int i = 0; i < GetCameraCount(); i++)
	{
		GetCameraAt(i)->Draw();
	}

}

void Scene::Update(float _dt)
{
	m_sceneRunTime += _dt;

	// GUI
	UpdateCameraGUI();
	UpdateLightGUI();
	UpdateParticleGUI();
	UpdateInstanceGUI();

	GetCamera()->Update(_dt);
}

void Scene::UpdateInstanceGUI()
{
	ImGui::Begin("Instances");
	int i = 0;
	std::list<Instance*> instances = GetInstances();
	for (auto it = instances.begin(); it != instances.end(); it++)
	{
		std::string iString = std::to_string(i);
		std::string headerName = iString;
		headerName.append(": Instance");
		const ImVec2 border = ImVec2(0, 0);
		ImGui::BeginGroup();
		if (ImGui::CollapsingHeader(headerName.c_str()))
		{
			Instance* inst = *it;

			bool isActive = inst->GetActive();
			ImGui::Checkbox((std::string("Is Active").append(iString)).c_str(), &isActive);
			inst->SetActive(isActive);

			glm::mat4 transform = inst->GetTransform();
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			
			glm::decompose(inst->GetTransform(), scale, rotation, translation, glm::vec3(0), glm::vec4(0));
			glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));

			ImGui::DragFloat3((std::string("Postition##").append(iString)).c_str(), &translation[0], 0.1f);
			ImGui::DragFloat3((std::string("Rotation##").append(iString)).c_str(), &euler[0], 0.1f, -180, 180);
			ImGui::DragFloat3((std::string("Scale##").append(iString)).c_str(), &scale[0], 0.1f);

			inst->SetTransform(inst->MakeTransform(translation, euler, scale));
		}
		ImGui::EndGroup();
		i++;
	}
	SetInstances(instances);
	ImGui::End();
}

void Scene::UpdateParticleGUI()
{
	ImGui::Begin("Particle Settings");
	int i = 0;
	for (auto it = m_particles.begin(); it != m_particles.end(); it++)
	{
		ParticleEmitter* particle = *it;

		std::string iString = std::to_string(i + 1);
		if (ImGui::CollapsingHeader(iString.append(std::string(": Particle")).c_str()))
		{
			float emitRate = particle->GetEmitRate();
			ImGui::DragFloat(std::string("Emission Rate##").append(iString).c_str(), &emitRate, 0.1f, 0.0001f, 1000.0f);
			particle->SetEmitRate(emitRate);

			float lifeSpanMax = particle->GetLifeSpanMax();
			ImGui::DragFloat(std::string("Life Span Max##").append(iString).c_str(), &lifeSpanMax, 0.1f, 0.0001f, 1000.0f);
			particle->SetLifeSpanMax(lifeSpanMax);

			float lifeSpanMin = particle->GetLifeSpanMin();
			ImGui::DragFloat(std::string("Life Span Min##").append(iString).c_str(), &lifeSpanMin, 0.1f, 0.0001f, 1000.0f);
			particle->SetLifeSpanMin(lifeSpanMin);

			float startSize = particle->GetStartSize();
			ImGui::DragFloat(std::string("Start Size##").append(iString).c_str(), &startSize, 0.1f, 0.0001f, 1000.0f);
			particle->SetStartSize(startSize);

			float endSize = particle->GetEndSize();
			ImGui::DragFloat(std::string("End Size##").append(iString).c_str(), &endSize, 0.1f, 0.0001f, 1000.0f);
			particle->SetEndSize(endSize);

			glm::vec4 startColour = particle->GetStartColour();
			ImGui::ColorEdit4(std::string("Start Colour##").append(iString).c_str(), &startColour[0], 0.1f);
			particle->SetStartColour(startColour);

			glm::vec4 endColour = particle->GetEndColour();
			ImGui::ColorEdit4(std::string("End Colour##").append(iString).c_str(), &endColour[0], 0.1f);
			particle->SetEndColour(endColour);
		}
		i++;
	}

	ImGui::End();

	m_pointLights[0].direction = glm::vec3(glm::cos(m_sceneRunTime), m_pointLights[0].direction.y, glm::sin(m_sceneRunTime * 2) / 2) * 10.0f;
	m_particles[0]->SetPosition(m_pointLights[0].direction);
}

void Scene::UpdateLightGUI()
{
	ImGui::Begin("Light Settings");
	if (ImGui::CollapsingHeader("Sunlight"))
	{
		ImGui::DragFloat3("Direction", &GetGlobalLight().direction[0], 0.1f, -0.1f, 1.0f);
		ImGui::ColorEdit3("Colour", &GetGlobalLight().colour[0]);
	}
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
}

void Scene::UpdateCameraGUI()
{
	ImGui::Begin("Camera Settings");
	ImGui::DragInt("Active Camera", &m_cameraIndex, 0.1f, 0, GetCameraCount() - 1);

	bool debugMode = GetCamera()->GetDebugMode();
	ImGui::Checkbox("Debug Camera", &debugMode);

	if (debugMode)
	{
		glm::vec4 camColour = GetCamera()->GetDebugColour();
		ImGui::ColorEdit4("Debug Colour", &camColour[0]);
		GetCamera()->SetDebugColour(camColour);
	}

	FlyCamera* flyCam = dynamic_cast<FlyCamera*>(GetCamera());
	if (flyCam != nullptr)
	{
		float speed = flyCam->GetSpeed();
		ImGui::DragFloat("FlyCam Speed", &speed, 1, 0, 10.0f);
		flyCam->SetSpeed(speed);
	}

	ImGui::End();
	GetCamera()->SetDebugMode(debugMode);
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

void Scene::SetParticleShader(aie::ShaderProgram* _shader)
{
	m_particleShader->SetShader(_shader);
}

void Scene::AddInstance(Instance* _instance)
{
	_instance->SetScene(this);
	m_instances.push_back(_instance);
}