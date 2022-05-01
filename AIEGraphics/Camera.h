#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	Camera(glm::vec3 _position);
	Camera(glm::vec3 _position, glm::vec2 _rotation);
	~Camera();

	virtual void Update(float _dt);
	virtual void Draw();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float w, float h);

	void SetPosition(const glm::vec3 _position);
	glm::vec3 GetPosition();

	void SetDebugColour(const glm::vec4 _colour);

	glm::vec4 GetDebugColour();

	glm::mat4 GetTransform(glm::vec3 _position, glm::vec3 _euler, glm::vec3 _scale);
	bool GetDebugMode() { return m_debugMode; }
	void SetDebugMode(bool isDebugging) { m_debugMode = isDebugging; }

protected:
	bool m_debugMode;
	float m_theta;
	float m_phi;
	glm::vec3 m_position;
	glm::vec4 m_debugColour;
	float m_lastMouseX;
	float m_lastMouseY;
};

