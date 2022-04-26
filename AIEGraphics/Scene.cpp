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

Scene::Scene(Camera** _cameras, int _cameraCount, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight) :
	m_cameras(_cameras), m_cameraCount(_cameraCount), m_cameraIndex(0), m_windowSize(_windowSize), m_globalDirlight(_light), m_ambientLight(_ambientLight)
{
	AddPointLights(Light({ 0,0,0 }, { 1,1,1 }, 1));
	m_particleShader = new EasyShader();
	//m_particleShaderFactory->SetShader();

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

	for (auto it = m_particles.begin(); it != m_particles.end(); it++)
	{
		ParticleEmitter* particle = *it;
		m_particleShader->OnBindShader();
		particle->Draw();
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
	GetCamera()->Update(_dt);

	// Light GUI
	ImGui::Begin("Light Settings");
	if (ImGui::CollapsingHeader("Sunlight"))
	{
		ImGui::DragFloat3("Direction", &GetGlobalLight().direction[0], 0.1f, -0.1f, 1.0f);
		ImGui::DragFloat3("Colour", &GetGlobalLight().colour[0], 0.1f, 0.0f, 2.0f);
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

	// Instances GUI
	ImGui::Begin("Instances");
	i = 0;
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
			auto transform = inst->GetTransform();

			// Active
			bool isActive = inst->GetActive();
			ImGui::Checkbox((std::string("Is Active").append(iString)).c_str(), &isActive);
			inst->SetActive(isActive);

			// Position
			float pos[] = { inst->GetPosition().x, inst->GetPosition().y, inst->GetPosition().z };

			// Rotation
			glm::quat quaternion = glm::quat_cast(inst->GetTransform());
			glm::vec3 euler = glm::degrees(glm::eulerAngles(quaternion));
			float eulerRot[] = { euler.x, euler.y, euler.z };

			// Scale
			glm::vec3 col1(transform[0][0], transform[1][0], transform[2][0]);
			glm::vec3 col2(transform[0][1], transform[1][1], transform[2][1]);
			glm::vec3 col3(transform[0][2], transform[1][2], transform[2][2]);

			float scaleX = glm::length(col1);
			float scaleY = glm::length(col2);
			float scaleZ = glm::length(col3);

			float scale[] = { scaleX, scaleY, scaleZ };

			//std::string posStrin
			ImGui::DragFloat3((std::string("Postition##").append(iString)).c_str(), pos, 0.1f);
			ImGui::DragFloat3((std::string("Rotation##").append(iString)).c_str(), eulerRot, 0.1f);
			ImGui::DragFloat3((std::string("Scale##").append(iString)).c_str(), scale, 0.1f);

			//ImGui::DragFloat3("Rotation", rot, 0.1f);			
			inst->SetTransform(inst->MakeTransform(glm::make_vec3(pos), glm::make_vec3(eulerRot), glm::make_vec3(scale)));
		}
		ImGui::EndGroup();
		i++;
	}
	SetInstances(instances);
	ImGui::End();
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


/*
*	preDefShader = new EasyShader(aie::ShaderProgram _shader).WithBindAction([=](auto s) { s.bindUniform("pvm",pvm);});)
*	instances = List<Tuple<Instance, EasyShader>>
*	instances.push_back(new Tuple<new Instance(...), preDefShader>)
*
*	instances.b.Bind()
*	instances.a.Draw()
*/