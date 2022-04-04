#include "AIEGraphicsApp.h"
#include "Instance.h"
#include "Scene.h"
#include "Gizmos.h"
#include "Input.h"
#include "Planet.h"
#include "SceneObject.h"
#include "FlyCamera.h"
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

	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.f);
	Light light;
	light.colour = { 2, 2, 2 };
	light.direction = { 1.0f, -0.1f, -0.1f };
	m_ambientLight = { 0.25f, 0.25f, 0.25f };
	m_cameraIndex = 0;
	m_cameras.push_back(new FlyCamera());
	m_cameras.push_back(new Camera());
	m_cameras[1]->SetPosition({-10,5,0});

	m_scene = new Scene(m_cameras[m_cameraIndex], glm::vec2(getWindowWidth(), getWindowHeight()), light, m_ambientLight);
	m_scene->AddPointLights(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 50);
	m_scene->AddPointLights(glm::vec3(-5, 3, 0), glm::vec3(0, 0, 1), 50);

	LaunchShader();

	m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, 0.1f, 1.0f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
	
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

	// particles
	m_emitter->Update(deltaTime, m_cameras[m_cameraIndex]->GetTransform(m_cameras[m_cameraIndex]->GetPosition(), {0,0,0}, {1,1,1}));

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	m_cameras[m_cameraIndex]->Update(deltaTime);

	ImGui::Begin("Light Settings");
	
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetGlobalLight().direction[0], 0.1f, -0.1f, 1.0f);
	ImGui::DragFloat3("Sunlight Colour", &m_scene->GetGlobalLight().colour[0], 0.1f, 0.0f, 2.0f);
	for (int i = 0; i < m_scene->GetLightCount(); i++)
	{
		std::string dirString = "Light Direction ";
		std::string colString = "Light Colour ";
		
		ImGui::DragFloat3(dirString.append(std::to_string(i + 1)).c_str(), &m_scene->GetPointLights()[i].direction[0], 0.1f, -0.1f, 1.0f);
		ImGui::DragFloat3(colString.append(std::to_string(i + 1)).c_str(), &m_scene->GetPointLights()[i].colour[0], 0.1f, 0.0f, 2.0f);
	}
	ImGui::End();

	FlyCamera* flyCam = dynamic_cast<FlyCamera*>(m_cameras[m_cameraIndex]);
	bool debugMode = m_cameras[m_cameraIndex]->GetDebugMode();

	ImGui::Begin("Camera Settings");
	ImGui::DragInt("Active Camera", &m_cameraIndex, 0.1f, 0, m_cameras.size() - 1);
	ImGui::Checkbox("Debug Camera", &debugMode);
	
	if (flyCam != nullptr)
	{
		float speed = flyCam->GetSpeed();
		ImGui::DragFloat("FlyCam Speed", &speed, 1, 0, 10.0f);
		flyCam->SetSpeed(speed);
	}

	ImGui::End();
	m_cameras[m_cameraIndex]->SetDebugMode(debugMode);
	m_scene->SetCamera(m_cameras[m_cameraIndex]);

	ImGui::Begin("Post-Processing Settings");
	ImGui::DragInt("Post-Processing Target", &m_postProcessingTarget, 0.1f, 0, 11);
	if (m_postProcessingTarget == 8)
		ImGui::DragInt("Pixel Strength", &m_pixelStrength, 0.1f, 0, 255);
	ImGui::End();

	ImGui::Begin("Instances");
	int i = 0;
	std::list<Instance*> instances = m_scene->GetInstances();
	for (auto it = instances.begin(); it != instances.end(); it++)
	{
		std::string iString = std::to_string(i);
		std::string headerName = iString;
		headerName.append(": Instance");
		const ImVec2 border = ImVec2(0, 0);
		ImGui::BeginGroup();
		if (ImGui::CollapsingHeader(headerName.c_str()))
		{
			Instance* inst = *it;
			auto transform = inst->GetTransform();
			
			// Position
			float pos[] = { inst->GetPosition().x, inst->GetPosition().y, inst->GetPosition().z };
			
			// Rotation
			glm::quat quaternion = glm::toQuat(inst->GetTransform());
			glm::vec3 euler = glm::eulerAngles(quaternion);
			float eulerRot[] = { euler.x, euler.y, euler.z };

			//glm::decom
			
			// Scale
			glm::vec3 col1(transform[0][0], transform[1][0], transform[2][0]);
			glm::vec3 col2(transform[0][1], transform[1][1], transform[2][1]);
			glm::vec3 col3(transform[0][2], transform[1][2], transform[2][2]);
			
			float scaleX = glm::length(col1);
			float scaleY = glm::length(col2);
			float scaleZ = glm::length(col3);

			float scale[] = {scaleX, scaleY, scaleZ};

			//std::string posStrin
			ImGui::DragFloat3((std::string("Postition##").append(iString)).c_str(), pos, 0.1f);
			ImGui::DragFloat3((std::string("Rotation##").append(iString)).c_str(), eulerRot, 0.1f);
			ImGui::DragFloat3((std::string("Scale##").append(iString)).c_str(), scale, 0.1f);

			//ImGui::DragFloat3("Rotation", rot, 0.1f);			
			inst->SetTransform(inst->MakeTransform(glm::make_vec3(pos), glm::make_vec3(eulerRot), glm::make_vec3(scale)));
		}
		ImGui::EndGroup();
		i++;
	}
	m_scene->SetInstances(instances);
	ImGui::End();
}

void AIEGraphicsApp::draw()
{
	// we need to bind our render target first
	m_renderTarget.bind();
	
	// wipe the screen to the background colour
	clearScreen();
	m_scene->Draw();

	for (int i = 0; i < m_cameras.size(); i++)
	{
		m_cameras[i]->Draw();
	}

	// update perspective based on screen size
	glm::mat4 projectionMatrix = m_cameras[m_cameraIndex]->GetProjectionMatrix(getWindowWidth(), getWindowHeight());
	glm::mat4 viewMatrix = m_cameras[m_cameraIndex]->GetViewMatrix();
	auto pvm = projectionMatrix * viewMatrix * mat4(1);

#pragma region Bunny

	//=================
	// Bunny
	//=================
	//m_modelTransform = m_bunnyTransform;
	//m_phongShader.bind();
	//pvm = projectionMatrix * viewMatrix * m_modelTransform;
	////// Bind the transform
	//m_phongShader.bindUniform("ProjectionViewModel", pvm);


	//// Bind light
	//m_phongShader.bindUniform("AmbientColour", m_ambientLight);
	//m_phongShader.bindUniform("LightColour", m_scene->GetGlobalLight().colour);
	//m_phongShader.bindUniform("LightDirection", m_scene->GetGlobalLight().direction);
	//m_phongShader.bindUniform("cameraPosition", m_cameras[m_cameraIndex]->GetPosition());

	////// Simple binding for lightind data based on model use
	//m_phongShader.bindUniform("ModelMatrix", m_modelTransform);

	//m_marbleTexture.bind(0);
	//m_phongShader.bindUniform("SeamlessTexture", 0);

	//m_hatchingTexture.bind(1);
	//m_phongShader.bindUniform("HatchingTexture", 1);

	//m_rampTexture.bind(2);
	//m_phongShader.bindUniform("RampTexture", 2);
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
	//=================

	m_gridTexture.bind(0);

	m_quadMesh.Draw();
	
	m_texturedShader.bind();


	// PLANET ---REMOVE---
	//sun->Draw();
	// -------------------

	//// Particles
	//pvm = projectionMatrix * viewMatrix * m_particleTransform;
	//m_particleShader.bind();
	//m_particleShader.bindUniform("ProjectionViewModel", pvm);
	//m_emitter->Draw();

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
	m_postShader.bindUniform("pixelateStrength", m_pixelStrength);

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

	/*if (!m_gridTexture.load("./textures/numbered_grid.tga"))
	{
		printf("Failed to load the grid texture, please check file path!\n");
		return false;
	}*/

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
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1
	};

	m_spearTransform = {
		1.0f,0,0,0,
		0,1.0f,0,0,
		0,0,1.0f,0,
		0,0,0,1.0f
	};

	m_particleTransform = {
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

	for (int i = 0; i < 10; i++)
		m_scene->AddInstance(new Instance(glm::vec3(i * 2, 0, 30), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), &m_bunnyMesh, &m_normalMapShader));

	return true;
}