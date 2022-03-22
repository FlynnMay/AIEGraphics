#include "AIEGraphicsApp.h"
#include "Instance.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "SceneObject.h"
#include "FlyCamera.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
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



	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
	Light light;
	light.colour = { 2, 2, 2 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };
	m_camera = new FlyCamera();
	m_camera->SetPosition({ -10,2,0 });

	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), light, m_ambientLight);
	m_scene->AddPointLights(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLights(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	LaunchShader();

	// PLANET ---REMOVE---
	//sun = new Planet(glm::vec3(0, 0, 0), 1.0f, RotationDirection::CLOCKWISE, glm::vec4(.75f, 0.15f, 0.75f, 1.0f));
	//earth = new Planet(glm::vec3(2, 0, 0), 0.8f, RotationDirection::CLOCKWISE, glm::vec4(0, 0.15f, 0.75f, 1.0f));
	//earth->SetParent(sun);
	//m_sceneObjects.push_back(sun);

	// -------------------

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

	// PLANET ---REMOVE---
	//glm::quat quaterion;
	//quaterion = glm::quat(glm::vec3(glm::radians(0.0f), glm::radians(0.0f), glm::radians(90.0f)));
	//sun->SetTransform(glm::toMat4(quaterion) * *sun->GetTransform());

	//glm::mat4 trans = *sun->GetTransform();
	////trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	//auto output = trans * model;
	//sun->SetTransform(output);
	//sun->Update(deltaTime);
	// -------------------

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// Grab the time since the application started
	float time = getTime();

	//// Rotate the light
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0.0f));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_camera->Update(deltaTime);

	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetGlobalLight().direction[0], 0.1f, -0.1f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetGlobalLight().colour[0], 0.1f, 0.0f, 2.0f);
	for (int i = 0; i < m_scene->GetLightCount(); i++)
	{
		ImGui::DragFloat3("Light Direction " + i, &m_scene->GetPointLights()[i].direction[0], 0.1f, -0.1f, 1.0f);
		ImGui::DragFloat3("Light Colour " + i, &m_scene->GetPointLights()[i].colour[0], 0.1f, 0.0f, 2.0f);
	}
	ImGui::End();
}

void AIEGraphicsApp::draw()
{

	// wipe the screen to the background colour
	clearScreen();
	m_scene->Draw();

	// update perspective based on screen size
	glm::mat4 projectionMatrix = m_camera->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	glm::mat4 viewMatrix = m_camera->GetViewMatrix();

#pragma region Bunny

	//=================
	// Bunny
	//=================
	//m_modelTransform = m_bunnyTransform;
	//m_phongShader.bind();
	auto pvm = projectionMatrix * viewMatrix * m_modelTransform;
	//// Bind the transform
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);


	//// Bind light
	//m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	//m_phongShader.bindUniform("LightColour", m_light.colour);
	//m_phongShader.bindUniform("LightDirection", m_light.direction);
	//m_phongShader.bindUniform("cameraPosition", m_camera->GetPosition());

	//// Simple binding for lightind data based on model use
	//m_phongShader.bindUniform("ModelMatrix", m_modelTransform);

	//// Draw mesh
	//m_bunnyMesh.draw();
	//=================

#pragma endregion

#pragma region Soul Spear
	////=================
	//// Soul Spear
	////=================
	//m_normalMapShader.bind();
	//m_modelTransform = m_spearTransform;
	//pvm = projectionMatrix * viewMatrix * m_modelTransform;
	//m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	//m_normalMapShader.bindUniform("diffuseTexture", 0);
	//m_normalMapShader.bindUniform("AmbientColour", m_ambientLight);
	//m_normalMapShader.bindUniform("LightColour", m_light.colour);
	//m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	//m_normalMapShader.bindUniform("cameraPosition", m_camera->GetPosition());
	//m_normalMapShader.bindUniform("ModelMatrix", m_modelTransform);

	////m_spearTexture.bind(0);
	//m_spearMesh.draw();
	////=================
#pragma endregion

	//=================
	// Grid Texture Quad
	//=================
	m_texturedShader.bind();
	m_modelTransform = m_quadTransform;
	pvm = projectionMatrix * viewMatrix * m_modelTransform;
	m_texturedShader.bindUniform("ProjectionViewModel", pvm);
	m_texturedShader.bindUniform("diffuseTexture", 0);
	m_gridTexture.bind(0);
	// Draw the quad
	//m_quadMesh.Draw();
	//=================


	// PLANET ---REMOVE---

	//sun->Draw();

	// -------------------

	Gizmos::draw(projectionMatrix * viewMatrix);
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

bool AIEGraphicsApp::LaunchShader()
{
#pragma region Shaders
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	m_texturedShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_texturedShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");

	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

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

#pragma endregion

#pragma region Mesh

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

	if (!m_gridTexture.load("./textures/numbered_grid.tga"))
	{
		printf("Failed to load the grid texture, please check file path!\n");
		return false;
	}

	//if (!m_spearTexture.load("./soulspear/soulspear_diffuse.tga"))
	//{
	//	printf("Failed to load the spear texture, please check file path!\n");
	//	return false;
	//}

	//if (!m_ironManTexture.load("./ironguy/M-COC_iOS_HERO_Tony_Stark_Iron_Man_Mark_VII_Body_D.png"))
	//{
	//	printf("Failed to load the ironman texture, please check file path!\n");
	//	return false;
	//}

#pragma endregion

	m_bunnyTransform = {
		0.05f,0,0,0,
		0,0.05f,0,0,
		0,0,0.05f,0,
		0,0,0,1
	};

	m_spearTransform = {
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f
	};

	m_otherTransform = {
		0.01f,0,0,0,
		0,0.01f,0,0,
		0,0,0.01f,0,
		0,0,0,0.01f
	};


	// Define 6 vertices for our two triangles
	Mesh::Vertex vertices[4];
	vertices[0].position = { -0.5f, 0 ,0.5f, 1.0f };
	vertices[1].position = { 0.5f, 0 ,0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0 ,-0.5f, 1.0f };
	vertices[3].position = { 0.5f, 0 ,-0.5f, 1.0f };

	unsigned int indices[6] = {
		0,1,2,2,1,3
	};
	//m_quadMesh.Initialise(4, vertices, 6, indices);
	//m_quadMesh.CreateGrid(2,2);

	m_quadMesh.InitialiseQuad();
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	}; // This is 10 units large

	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, i * 30, i * 30), glm::vec3(1, 1, 1), &m_spearMesh, &m_normalMapShader));

	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0.1f * i, 0.1f * i, 0.1f * i), &m_otherMesh, &m_normalMapShader));
	return true;
}