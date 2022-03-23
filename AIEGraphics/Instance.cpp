#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "OBJMesh.h"
#include "Camera.h"
#include <Texture.h>
#include "AIEGraphicsApp.h"

Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader) : m_transform(_transform), m_mesh(_mesh), m_shader(_shader)
{
}

Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader) : m_mesh(_mesh), m_shader(_shader)
{
    m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

void Instance::Draw(Camera* _camera, float _windowWidth, float _windowHeight, glm::vec3 _ambientLight, Light* _light)
{
}

void Instance::Draw(Scene* _scene)
{
    // set the render pipeline
    m_shader->bind();

    // bind the transforms and other relevant uniforms
    auto pvm = _scene->GetCamera()->GetProjectionMatrix(
        _scene->GetWindowSize().x,
        _scene->GetWindowSize().y
    ) * _scene->GetCamera()->GetViewMatrix() * m_transform;
    m_shader->bindUniform("ProjectionViewModel", pvm);
    m_shader->bindUniform("ModelMatrix", m_transform);

    // bind the lighting and camera uniforms
    m_shader->bindUniform("AmbientColour", _scene->GetAmbientLight());
    m_shader->bindUniform("LightColour", _scene->GetGlobalLight().colour);
    m_shader->bindUniform("LightDirection", _scene->GetGlobalLight().direction);

    m_shader->bindUniform("cameraPosition", _scene->GetCamera()->GetPosition());

    int numberOfLights = _scene->GetLightCount();
    m_shader->bindUniform("numLights", numberOfLights);
    m_shader->bindUniform("PointLightPositions", numberOfLights, _scene->GetPointLightPositions());
    m_shader->bindUniform("PointLightColors", numberOfLights, _scene->GetPointLightColours());

    m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 _positon, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), _positon)
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), _scale);
}
