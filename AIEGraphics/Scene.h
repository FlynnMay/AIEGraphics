#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>

class Camera;
class Instance;

const int MAX_LIGHTS = 12;

struct Light
{
	Light(){}
	Light(glm::vec3 _pos, glm::vec3 _colour, float intensity)
	{
		direction = _pos;
		colour = _colour * intensity;
	}
	glm::vec3 direction;
	glm::vec3 colour;
};

class Scene
{
public:
	Scene(Camera* _camera, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight);
	~Scene();

	void AddInstance(Instance* _instance);
	void Draw();

	std::list<Instance*> GetInstances() { return m_instances; }
	void SetInstances(std::list<Instance*> _instances) { m_instances = _instances; }
	Camera* GetCamera() { return m_camera; }
	void SetCamera(Camera* _camera) { m_camera = _camera; }
	glm::vec2 GetWindowSize() { return m_windowSize; }
	Light& GetGlobalLight() { return m_globalDirlight; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	int GetLightCount() { return (int)m_pointLights.size(); }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColours() { return &m_pointLightColours[0]; }
	void AddPointLights(Light _light) { m_pointLights.push_back(_light); }
	void AddPointLights(glm::vec3 _dir, glm::vec3 _colour, float _intensity) { m_pointLights.push_back(*new Light(_dir, _colour, _intensity)); }

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_globalDirlight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];
};

