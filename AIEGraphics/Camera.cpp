#include "Camera.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <imgui.h>

Camera::Camera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(0, 0, 0);
	m_debugMode = false;
	m_debugColour = glm::vec4(rand() % 101 / 100.0f, rand() % 101 / 100.0f, rand() % 101 / 100.0f, 1);
}

Camera::Camera(glm::vec3 _position)
{
	m_theta = 0;
	m_phi = 0;
	m_position = _position;
	m_debugMode = false;
	m_debugColour = glm::vec4(rand() % 101 / 100.0f, rand() % 101 / 100.0f, rand() % 101 / 100.0f, 1);
}

Camera::Camera(glm::vec3 _position, glm::vec2 _rotation)
{
	m_theta = _rotation.x;
	m_phi = _rotation.y;
	m_position = _position;
	m_debugMode = false;
	m_debugColour = glm::vec4(rand() % 101 / 100.0f, rand() % 101 / 100.0f, rand() % 101 / 100.0f, 1);
}

Camera::~Camera()
{
}

void Camera::Update(float _dt)
{
}

void Camera::Draw()
{
	if (!m_debugMode)
		return;

	aie::Gizmos::addCylinderFilled(GetPosition(), 0.1f, .1f, 16, m_debugColour);
}

glm::mat4 Camera::GetViewMatrix()
{
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
	return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.f);
}

void Camera::SetPosition(const glm::vec3 _position)
{
	m_position = _position;
}

glm::vec3 Camera::GetPosition()
{
	return m_position;
}

void Camera::SetDebugColour(const glm::vec4 _colour)
{
	m_debugColour = _colour;
}

glm::vec4 Camera::GetDebugColour()
{
	return m_debugColour;
}

glm::mat4 Camera::GetTransform(glm::vec3 _position, glm::vec3 _euler, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _position)
		* glm::rotate(glm::mat4(1), glm::radians(_euler.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(_euler.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(_euler.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), _scale);
}