#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>

class Camera;
class Instance;
class ParticleEmitter;
class EasyShader;

namespace aie
{
	class ShaderProgram;
}

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
	Scene(Camera** _cameras, int _cameraCount, glm::vec2 _windowSize, Light& _light, glm::vec3 _ambientLight);
	~Scene();

	// Adds the instance to the scene and updates the instances scene
	void AddInstance(Instance* _instance);

	void Draw();
	
	void Update(float _dt);

	// Updates the instance debug UI
	void UpdateInstanceGUI();
	
	// Updates the particles debug UI
	void UpdateParticleGUI();
	
	// Updates the lights debug UI
	void UpdateLightGUI();

	// Updates the cameras debug UI
	void UpdateCameraGUI();

	// returns a list of instances
	std::list<Instance*> GetInstances() { return m_instances; }
	
	// overwrites the list of current instances 
	void SetInstances(std::list<Instance*> _instances) { m_instances = _instances; }
	
	// returns the amount of cameras
	int GetCameraCount() { return m_cameraCount; }

	// returns the current cameras index
	int GetCameraIndex() { return m_cameraIndex; }
	
	// returns the current camera
	Camera* GetCamera();

	// returns the camera at the index
	Camera* GetCameraAt(int _index);

	// Sets the camera at the given index
	void SetCamera(Camera* _camera, int _index);
	
	// Adds a camera to the end of the array
	void AddCamera(Camera* _camera);

	glm::vec2 GetWindowSize() { return m_windowSize; }

	Light& GetGlobalLight() { return m_globalDirlight; }
	glm::vec3 GetAmbientLight() { return m_ambientLight; }

	// returns a list of the scenes point lights
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	
	// retuns how many point lights are in the scene
	int GetLightCount() { return (int)m_pointLights.size(); }
	
	// returns an array of all point light positions in the scene
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	
	// returns an array of all point light colours in the scene
	glm::vec3* GetPointLightColours() { return &m_pointLightColours[0]; }
	
	// adds a point light to the scene
	void AddPointLight(Light _light) { m_pointLights.push_back(_light); }
	
	// creates and adds a new point light to the scene
	void AddPointLight(glm::vec3 _dir, glm::vec3 _colour, float _intensity) { m_pointLights.push_back(*new Light(_dir, _colour, _intensity)); }

	// adds a particle to the scene
	void AddParticle(ParticleEmitter* _particle);
	
	// sets the shader used by the scenes particles
	void SetParticleShader(aie::ShaderProgram* _shader);

protected:
	Camera* m_camera;
	glm::vec2 m_windowSize;
	Light m_globalDirlight;
	std::vector<Light> m_pointLights;
	glm::vec3 m_ambientLight;
	std::list<Instance*> m_instances;
	std::vector<ParticleEmitter*> m_particles;

	EasyShader* m_particleShader;

	Camera** m_cameras;
	int m_cameraCount;
	int m_cameraIndex;

	glm::vec3 m_pointLightPositions[MAX_LIGHTS];
	glm::vec3 m_pointLightColours[MAX_LIGHTS];

	float m_sceneRunTime = 0;
};

