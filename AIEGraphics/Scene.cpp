#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight) :
	m_camera(_camera), m_windowSize(_windowSize), m_globalDirlight(_light), m_ambientLight(_ambientLight)
{
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
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}
