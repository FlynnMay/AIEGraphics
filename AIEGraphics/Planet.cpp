#include <Gizmos.h>
#include "Planet.h"
#include <glm/ext.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

Planet::Planet(glm::vec3 position, float radius, RotationDirection rotationDirection, glm::vec4 colour) : 
	m_position(position), m_radius(radius), m_rotationDirection(rotationDirection), m_colour(colour), SceneObject()
{
	m_transform = new glm::mat4(1.0f);

	//*m_transform = glm::translate(*m_transform, glm::vec3(0, 0, 0)); //position = 0,0,0
	//*m_transform = glm::rotate(*m_transform, glm::radians(0.0f), glm::vec3(1, 0, 0));//rotation x = 0.0 degrees
	//*m_transform = glm::rotate(*m_transform, glm::radians(0.0f), glm::vec3(0, 1, 0));//rotation y = 0.0 degrees
	//*m_transform = glm::rotate(*m_transform, glm::radians(0.0f), glm::vec3(0, 0, 1));//rotation z = 0.0 degrees
	//*m_transform = glm::scale(*m_transform, glm::vec3(1, 1, 1));//scale = 1,1,1

	*m_transform = glm::translate(*m_transform, position);
}

Planet::~Planet()
{
}

void Planet::OnUpdate(float dt)
{
	//*m_transform = glm::translate(*m_transform, glm::vec3(0, 10, 0) * dt);
	//*m_transform = glm::rotate(*m_transform, glm::radians(180.0f) * dt, glm::vec3(0, 0, 1));
}

void Planet::OnDraw()
{
	glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;

	auto gt = GetGlobalTransform();

	glm::decompose(gt, scale, rotation, translation, skew, perspective);
	aie::Gizmos::addSphere(translation, m_radius, 16, 16, m_colour);
}
