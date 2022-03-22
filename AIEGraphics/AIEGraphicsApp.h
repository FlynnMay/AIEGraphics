#pragma once

#include <glm/mat4x4.hpp>
#include <list>
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"

class Scene;
class SceneObject;
class AIEGraphicsApp : public aie::Application {
public:

	AIEGraphicsApp();
	virtual ~AIEGraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	//void SetupShaders(aie::ShaderProgram shader, const char* charString);

	bool LaunchShader();


	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normalMapShader;
	
	aie::Texture m_gridTexture;
	aie::Texture m_spearTexture;
	aie::Texture m_ironManTexture;

	Mesh m_quadMesh;
	
	glm::mat4 m_modelTransform;
	glm::mat4 m_quadTransform;
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

	aie::OBJMesh m_otherMesh;
	glm::mat4 m_otherTransform;

	// camera transforms
	//glm::mat4	m_viewMatrix;
	//glm::mat4	m_projectionMatrix;

	// camera
	Camera* m_camera;

	// other stuff
	SceneObject* sun;
	SceneObject* earth;
	std::list<SceneObject*> m_sceneObjects;

	float m_zoom = 10.0f;
	float m_speed = 10.0f;
	float m_rotationSpeed = 45.0f;
	float m_rotation = 0.0f;
	glm::vec3 m_position = glm::vec3(0);
	
	glm::vec3 m_ambientLight;

	Scene* m_scene;
};