#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	virtual void Update(float _dt);
	virtual void Draw();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	void SetPosition(const glm::vec3 _position);
	glm::vec3 GetPosition();

	void SetPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void SetLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	glm::mat4 GetWorldTransform();
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	glm::mat4 GetProjectionView();

	bool m_debugMode;

protected:
	float m_theta;
	float m_phi;
	glm::vec3 m_position;
	float m_lastMouseX;
	float m_lastMouseY;

private:

	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;
	glm::mat4 m_projectionTransform;
	glm::mat4 m_projectionViewTransform;
	
	void UpdateProjectionViewTransform();
};

