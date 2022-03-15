#include "AIEGraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "SceneObject.h"
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
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	// PLANET ---REMOVE---
	sun = new Planet(glm::vec3(0, 0, 0), 1.0f, RotationDirection::CLOCKWISE, glm::vec4(.75f, 0.15f, 0.75f, 1.0f));
	earth = new Planet(glm::vec3(2, 0, 0), 0.8f, RotationDirection::CLOCKWISE, glm::vec4(0, 0.15f, 0.75f, 1.0f));
	earth->SetParent(sun);
	m_sceneObjects.push_back(sun);

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
	
	glm::mat4 trans = *sun->GetTransform();
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	auto output = trans * model;
	sun->SetTransform(output);
	sun->Update(deltaTime);
	// -------------------

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void AIEGraphicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective based on screen size
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	//Bind the shader
	m_shader.bind();

	// Bind the transform
	auto pvm = m_projectionMatrix * m_viewMatrix * m_quadTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);

	// Draw the quad
	m_quadMesh.Draw();

	// PLANET ---REMOVE---

	sun->Draw();

	// -------------------

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}

bool AIEGraphicsApp::LaunchShader()
{
	m_shader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_shader.link()) {
		printf("Simple Shader Error: %s\n", m_shader.getLastError());
		return false;
	}

	m_quadMesh.InitialiseQuad();
	m_quadTransform = {
		10,  0,  0,  0,
		 0, 10,  0,  0,
		 0,  0, 10,  0,
		 0,  0,  0,  1
	}; // This is 10 units large

	return true;
}