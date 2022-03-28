#include "Camera.h"
#include <Gizmos.h>
#include <glm/ext.hpp>

Camera::Camera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(0, 0, 0);
	m_debugMode = false;
}

Camera::~Camera()
{
}

void Camera::Update(float _dt)
{
	UpdateProjectionViewTransform();
}

void Camera::Draw()
{
	if (!m_debugMode)
		return;

	aie::Gizmos::addSphere(GetPosition(), 0.1f, 10, 10, { 1,0,0,1 });
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

void Camera::SetPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	m_projectionTransform = glm::perspective(glm::pi<float>() * fieldOfView, aspectRatio, near, far);
}

void Camera::SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
}

glm::mat4 Camera::GetWorldTransform()
{
	return m_worldTransform;
}

glm::mat4 Camera::GetView()
{
	//float thetaR = glm::radians(m_theta);
	//float phiR = glm::radians(m_phi);
	//glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	//return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
	return m_viewTransform;
}

glm::mat4 Camera::GetProjection()
{
	return m_projectionTransform;
}

glm::mat4 Camera::GetProjectionView()
{
	return m_projectionViewTransform;
}

void Camera::UpdateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
