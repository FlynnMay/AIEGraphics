#include "AIEGraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "SceneObject.h"
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

AIEGraphicsApp::AIEGraphicsApp() {

}

AIEGraphicsApp::~AIEGraphicsApp() {

}

bool AIEGraphicsApp::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	LaunchShader();

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);

	m_light.colour = { 1, 1, 0 };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };


	// PLANET ---REMOVE---
	//sun = new Planet(glm::vec3(0, 0, 0), 1.0f, RotationDirection::CLOCKWISE, glm::vec4(.75f, 0.15f, 0.75f, 1.0f));
	//earth = new Planet(glm::vec3(2, 0, 0), 0.8f, RotationDirection::CLOCKWISE, glm::vec4(0, 0.15f, 0.75f, 1.0f));
	//earth->SetParent(sun);
	//m_sceneObjects.push_back(sun);

	// -------------------

	return true;
}

void AIEGraphicsApp::shutdown() {

	Gizmos::destroy();
}

void AIEGraphicsApp::update(float deltaTime) {

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
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

	// GRab the time since the application started
	float time = getTime();

	//// Rotate the light
	//m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0.0f));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	//Input UP/DOWN - ZOOM
	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_zoom -= 0.5f;

	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_zoom += 0.5f;

	//Input LEFT/RIGHT - ROTATION
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_rotation += deltaTime * glm::radians(m_rotationSpeed);

	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_rotation -= deltaTime * glm::radians(m_rotationSpeed);

	vec3 forward = -vec3(cos(m_rotation), 0, sin(m_rotation));
	vec3 right = vec3(forward.z, 0, -forward.x);

	//Input W/S - Z MOVE
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * deltaTime * m_speed;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * deltaTime * m_speed;

	//Input A/D - X MOVE
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position += right * deltaTime * m_speed;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position -= right * deltaTime * m_speed;

	m_viewMatrix = glm::lookAt(vec3(cos(m_rotation), 1, sin(m_rotation)) * m_zoom + m_position, m_position, vec3(0, 1, 0));

	ImGui::Begin("Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -0.1f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_light.colour[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();
}

void AIEGraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	//Bind the shader
	//m_shader.bind();

	// Bind the transform
	//auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_modelTransform = m_bunnyTransform;
	auto pvm = m_projectionMatrix * m_viewMatrix * m_modelTransform;
	//m_shader.bindUniform("ProjectionViewModel", pvm);
	m_phongShader.bindUniform("ProjectionViewModel", pvm);


	// Draw mesh
	//m_bunnyMesh.draw();

	m_phongShader.bind();

	// Bind light
	m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	m_phongShader.bindUniform("LightColour", m_light.colour);
	m_phongShader.bindUniform("LightDirection", m_light.direction);
	m_phongShader.bindUniform("cameraPosition", vec3(glm::inverse(m_viewMatrix)[3]));


	// Bind the transform
	pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_phongShader.bindUniform("ProjectionViewModel", pvm);

	// Simple binding for lightind data based on model use
	m_phongShader.bindUniform("ModelMatrix", m_modelTransform);

	// Draw the quad
	//m_quadMesh.Draw();

	// PLANET ---REMOVE---

	//sun->Draw();

	// -------------------

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}

bool AIEGraphicsApp::LaunchShader()
{
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (!m_shader.link()) {
		printf("Simple Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	if (!m_phongShader.link()) {
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if (!m_bunnyMesh.load("./stanford/bunny.obj"))
	{
		printf("Bunny Mesh Error!\n");
		return false;
	}
	
	m_bunnyTransform = {
		0.5f,0,0,0,
		0,0.5f,0,0,
		0,0,0.5f,0,
		0,0,0,1
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

	return true;
}