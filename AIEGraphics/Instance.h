#pragma once
#include <glm/glm.hpp>

class Camera;
struct Light;
class Scene;
class EasyShader;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	// sets the "easy shader" to bind uniforms for a normal map shader
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _normalMapShader);

	// sets the "easy shader" to bind uniforms for a normal map shader, uses vector3's to make a transform
	Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _normalMapShader);

	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, EasyShader* _shader);

	Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, EasyShader* _shader);

	void Draw();

	// Used to bind uniforms for "default" objects using the normal map shader 
	void BindDefaultNormalMapShaderUniforms(aie::ShaderProgram* shader);

	static glm::mat4 MakeTransform(glm::vec3 _positon, glm::vec3 _eulerAngles, glm::vec3 _scale);
	glm::vec3 GetPosition() { return m_transform[3]; }
	glm::mat4 GetTransform() { return m_transform; }
	void SetTransform(glm::mat4 _transform) { m_transform = _transform; }
	void SetActive(bool _active) { m_isActive = _active; }
	bool GetActive() { return m_isActive; }
	void SetScene(Scene* _scene) { m_scene = _scene; }

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	aie::ShaderProgram* m_shader;
	Scene* m_scene;
	EasyShader* m_easyShader;
	bool m_isActive = true;
};