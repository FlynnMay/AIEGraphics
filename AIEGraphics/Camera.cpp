#include "Camera.h"
#include <glm/ext.hpp>
#include <Input.h>

Camera::Camera()
{
	m_theta = 0;
	m_phi = 0;
	m_position = glm::vec3(0, 0, 0);
}

Camera::~Camera()
{
}

void Camera::Update(float _dt)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// use the WASD, ZX keys to the move camera around
	if (input->isKeyDown(aie::INPUT_KEY_X))
		m_position += up * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_Z))
		m_position += -up * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position += -forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position += -right * _dt;

	// get the current mouse coordinates
	float mx = input->getMouseX();
	float my = input->getMouseY();
	const float turnSpeed = 0.1f;

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += turnSpeed * (mx - m_lastMouseX);
		m_phi += turnSpeed * (my - m_lastMouseY);
	}

	m_phi = glm::clamp(m_phi, -70.0f, 70.0f);

	m_lastMouseX = mx;
	m_lastMouseY = my;

	UpdateProjectionViewTransform();
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
	//m_worldTransform

}

glm::vec3 Camera::GetPosition()
{
	return glm::vec3();
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
