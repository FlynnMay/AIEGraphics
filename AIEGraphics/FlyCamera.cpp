#include "FlyCamera.h"
#include <Input.h>

void FlyCamera::Update(float _dt)
{
	aie::Input* input = aie::Input::getInstance();
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// calculate the forwards and right axes and up axis for the camera
	glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
	glm::vec3 right(-sin(thetaR), 0, cos(thetaR));

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
}
