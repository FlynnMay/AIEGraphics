#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
public:
	void Update(float _dt);
	void SetSpeed(float _speed) { m_speed = _speed; }
	float GetSpeed() { return m_speed; }

private:
	float m_speed = 1.0f;
	glm::vec3 up = glm::vec3(0,1,0);
};

