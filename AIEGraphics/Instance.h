#pragma once
#include <glm/glm.hpp>

class Camera;
struct Light;
class Scene;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader);
	Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader);

	void Draw(Camera* _camera, float _windowWidth, float _windowHeight, glm::vec3 _ambientLight, Light* _light);
	void Draw(Scene* _scene);

	static glm::mat4 MakeTransform(glm::vec3 _positon, glm::vec3 _eulerAngles, glm::vec3 _scale);
	glm::vec3 GetPosition() { return m_transform[3]; }
	glm::mat4 GetTransform() { return m_transform; }
	void SetTransform(glm::mat4 _transform) { m_transform = _transform; }

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
};

