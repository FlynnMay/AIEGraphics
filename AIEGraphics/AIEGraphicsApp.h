#pragma once

#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include <glm/mat4x4.hpp>
#include <list>

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
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	// other stuff
	SceneObject* sun;
	SceneObject* earth;
	std::list<SceneObject*> m_sceneObjects;
};