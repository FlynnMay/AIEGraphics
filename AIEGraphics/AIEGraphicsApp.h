#pragma once
#include <glm/mat4x4.hpp>
#include <list>
#include <vector>
#include "RenderTarget.h"
#include "Application.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Camera.h"

class Scene;
class SceneObject;
class ParticleEmitter;
class AIEGraphicsApp : public aie::Application {
public:

	AIEGraphicsApp();
	virtual ~AIEGraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	bool LaunchShader();

	ParticleEmitter* m_emitter;

	aie::ShaderProgram m_shader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_texturedShader;
	aie::ShaderProgram m_normalMapShader;
	aie::ShaderProgram m_postShader;
	aie::ShaderProgram m_particleShader;
	
	int m_postProcessingTarget = 0;

	aie::RenderTarget m_renderTarget;

	aie::Texture m_gridTexture;
	aie::Texture m_spearTexture;

	aie::Texture m_marbleTexture;
	aie::Texture m_hatchingTexture;
	aie::Texture m_rampTexture;

	glm::mat4 m_modelTransform;
	glm::mat4 m_quadTransform;

	Mesh m_quadMesh;
	Mesh m_screenQuad;
	aie::OBJMesh m_bunnyMesh;
	aie::OBJMesh m_spearMesh;
	aie::OBJMesh m_otherMesh;

	float m_zoom = 10.0f;
	float m_speed = 10.0f;
	float m_rotationSpeed = 45.0f;
	float m_rotation = 0.0f;
	int m_pixelStrength = 1;
	int m_blurStrength = 1;

	glm::vec3 m_ambientLight;

	Scene* m_scene;
};