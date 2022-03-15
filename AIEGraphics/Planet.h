#pragma once
#include <glm/glm.hpp>
#include "SceneObject.h"
enum class RotationDirection
{
	COUNTERCLOCKWISE = -1,
	UNASSIGNED = 0,	
	CLOCKWISE = 1
};

class Planet : public SceneObject
{
public:
	Planet(glm::vec3 position, float radius, RotationDirection rotationDirection, glm::vec4 colour);
	~Planet();
	
	void OnUpdate(float dt);
	void OnDraw();

protected:
	glm::vec3 m_position;
	float m_radius;
	RotationDirection m_rotationDirection;
	glm::vec4 m_colour;
};

