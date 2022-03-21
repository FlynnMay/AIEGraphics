#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
public:
	void Update(float _dt);
	void SetSpeed(float _speed) { m_speed = _speed; }

private:
	float m_speed;
	glm::vec3 up;
};

