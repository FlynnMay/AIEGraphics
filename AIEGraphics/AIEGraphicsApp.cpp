#include "AIEGraphicsApp.h"
#include "Instance.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Input.h"
#include "FlyCamera.h"
#include "EasyShader.h"
#include "ParticleEmitter.h"
#include <string>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>


using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

AIEGraphicsApp::AIEGraphicsApp()
{

}

AIEGraphicsApp::~AIEGraphicsApp()
{

}

bool AIEGraphicsApp::startup()
{

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	Light light;
	light.colour = { 2, 2, 2 };
	light.direction = { 1.0f, -0.1f, -0.1f };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };

	// setup cameras
	std::vector <Camera*> cams;
	cams.push_back(new FlyCamera());
	cams.push_back(new Camera({ -15,5,0 }, { 0, 0 }));
	cams.push_back(new Camera({ 15, 5, 0}, { 180, 0 }));
	cams.push_back(new Camera({ 0,5,15 }, { 270, 0 }));
	cams.push_back(new Camera({ 0,5,-15 }, { 90, 0 }));

	// setup scene
	m_scene = new Scene(cams, glm::vec2(getWindowWidth(), getWindowHeight()), light, m_ambientLight);
	m_scene->AddPointLight(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLight(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	// Launch shaders return if the process fails
	if (!LaunchShader())
		return false;

	// setup particle
	m_scene->SetParticleShader(&m_particleShader);
	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	m_scene->AddParticle(m_emitter);

	return true;
}

void AIEGraphicsApp::shutdown()
{
	Gizmos::destroy();
}

void AIEGraphicsApp::update(float deltaTime)
{
	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Grab the time since the application started
	float time = getTime();

	//// Rotate the light
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0.0f));

	// particles
	m_emitter->Update(deltaTime, m_scene->GetCamera()->GetTransform(m_scene->GetCamera()->GetPosition(), { 0,0,0 }, { 1,1,1 }));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_scene->Update(deltaTime);

	ImGui::Begin("Post-Processing Settings");
	ImGui::DragInt("Post-Processing Target", &m_postProcessingTarget, 0.1f, 0, 11);
	if (m_postProcessingTarget == 1)
		ImGui::DragInt("Blur Strength", &m_blurStrength, 0.1f, 0, 25);
	else if (m_postProcessingTarget == 8)
		ImGui::DragInt("Pixel Strength", &m_pixelStrength, 0.1f, 0, 255);
	ImGui::End();
}

void AIEGraphicsApp::draw()
{
	// we need to bind our render target first
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	m_scene->Draw();

	// update perspective based on screen size
	glm::mat4 projectionMatrix = m_scene->GetCamera()->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	glm::mat4 viewMatrix = m_scene->GetCamera()->GetViewMatrix();
	auto pvm = projectionMatrix * viewMatrix * mat4(1);

	//=================
	// Grid Texture Quad
	//=================
	m_texturedShader.bind();
	pvm = projectionMatrix * viewMatrix * m_quadTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);
	// Draw the quad
	//=================

	m_gridTexture.bind(0);

	m_quadMesh.Draw();

	m_texturedShader.bind();

	for (int i = 0; i < m_scene->GetLightCount(); i++)
	{
		Light light = m_scene->GetPointLights()[i];
		//Gizmos::addSphere()
	}

	Gizmos::draw(projectionMatrix * viewMatrix);
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	// Unbind the target to return it to the back of the buffer
	m_renderTarget.unbind();
	clearScreen();

	// Bind the post processing shader and texture
	m_postShader.bind();
	m_postShader.bindUniform("colourTarget", 0);
	m_postShader.bindUniform("postProcessTarget", m_postProcessingTarget);
	m_postShader.bindUniform("width", (int)getWindowWidth());
	m_postShader.bindUniform("height", (int)getWindowHeight());

	// Pixelate Uniforms
	m_postShader.bindUniform("pixelateStrength", m_pixelStrength);

	// Blur Uniforms
	m_postShader.bindUniform("blurStrength", m_blurStrength);

	m_renderTarget.getTarget(0).bind(0);
	m_screenQuad.Draw();
}

bool AIEGraphicsApp::LaunchShader()
{
	if (!m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()))
	{
		printf("[Render Target] Error!\n");
		return false;
	}

#pragma region Shaders
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phongExt.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phongExt.frag");

	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	m_postShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/advancedPost.vert");
	m_postShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/advancedPost.frag");

	m_particleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/particle.vert");
	m_particleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/particle.frag");

	if (!m_shader.link())
	{
		printf("[Simple] Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	if (!m_phongShader.link())
	{
		printf("[Phong] Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if (!m_texturedShader.link())
	{
		printf("[Textured] Shader Error: %s\n", m_texturedShader.getLastError());
		return false;
	}

	if (!m_normalMapShader.link())
	{
		printf("[Normal Map] Shader Error: %s\n", m_normalMapShader.getLastError());
		return false;
	}

	if (!m_postShader.link())
	{
		printf("[Post Processing] Shader Error: %s\n", m_postShader.getLastError());
		return false;
	}

	if (!m_particleShader.link())
	{
		printf("[Particle] Shader Error: %s\n", m_particleShader.getLastError());
		return false;
	}

#pragma endregion

#pragma region Mesh

	m_screenQuad.InitialiseFullScreenQuad();

	if (!m_bunnyMesh.load("./stanford/bunny.obj"))
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}

	if (!m_spearMesh.load("./soulspear/soulspear.obj", true, true))
	{
		printf("Spear Mesh Error!\n");
		return false;
	}

	if (!m_otherMesh.load("./Gun/P88.obj", true, true))
	{
		printf("Other Mesh Error!\n");
		return false;
	}

#pragma endregion

#pragma region Texture

	if (!m_gridTexture.load("./Sobel.png"))
	{
		printf("Failed to load the grid texture, please check file path!\n");
		return false;
	}

	if (!m_marbleTexture.load("./textures/marble2.jpg"))
	{
		printf("Failed to load the marble texture, please check file path!\n");
		return false;
	}

	if (!m_hatchingTexture.load("./textures/Ramp02.png"))
	{
		printf("Failed to load the hatching texture, please check file path!\n");
		return false;
	}

	if (!m_rampTexture.load("./textures/ramps.png", true))
	{
		printf("Failed to load the hatching texture, please check file path!\n");
		return false;
	}

#pragma endregion

	// Define 6 vertices for our two triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0 ,0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0 ,0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0 ,-0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0 ,-0.5f, 1.0f };

	unsigned int indices[6] = {
		0,1,2,2,1,3
	};

	m_quadMesh.InitialiseQuad();

	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	}; // This is 10 units large

	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, -10), glm::vec3(0, i * 30, i * 30), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));

	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(0, i * 2, 0), glm::vec3(0, 0, 0), glm::vec3(0.1f * i, 0.1f * i, 0.1f * i), &m_otherMesh, &m_normalMapShader));

	for (int i = 0; i < 10; i++) {
		EasyShader* easyPhongShader = new EasyShader(&m_phongShader);

		Instance* inst = new Instance(glm::vec3(i * 10, 0, 10), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &m_bunnyMesh, easyPhongShader);

		easyPhongShader->SetOnShaderBind([=](aie::ShaderProgram* s) {
			glm::mat4 projectionMatrix = m_scene->GetCamera()->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
			glm::mat4 viewMatrix = m_scene->GetCamera()->GetViewMatrix();
			auto pvm = projectionMatrix * viewMatrix * inst->GetTransform();

			// Bind the transform
			m_phongShader.bindUniform("ProjectionViewModel", pvm);


			// Bind light
			m_phongShader.bindUniform("AmbientColour", m_ambientLight);
			m_phongShader.bindUniform("LightColour", m_scene->GetGlobalLight().colour);
			m_phongShader.bindUniform("LightDirection", m_scene->GetGlobalLight().direction);
			m_phongShader.bindUniform("cameraPosition", m_scene->GetCamera()->GetPosition());

			// Simple binding for lightind data based on model use
			m_phongShader.bindUniform("ModelMatrix", inst->GetTransform());

			m_marbleTexture.bind(0);
			m_phongShader.bindUniform("SeamlessTexture", 0);

			m_hatchingTexture.bind(1);
			m_phongShader.bindUniform("HatchingTexture", 1);

			m_rampTexture.bind(2);
			m_phongShader.bindUniform("RampTexture", 2);
			});
		m_scene->AddInstance(inst);
	}

	return true;
}