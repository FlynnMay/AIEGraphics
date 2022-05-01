#include "Instance.h"
#include <glm/ext.hpp>
#include "Scene.h"
#include "OBJMesh.h"
#include "Camera.h"
#include <Texture.h>
#include "AIEGraphicsApp.h"
#include "EasyShader.h"

Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader) : m_transform(_transform), m_mesh(_mesh)
{
    m_easyShader = new EasyShader(_shader);
    m_easyShader->SetOnShaderBind([=](aie::ShaderProgram* s) {
            BindDefaultNormalMapShaderUniforms(s);
        });
}

Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, aie::ShaderProgram* _shader) : m_mesh(_mesh)
{
    m_easyShader = new EasyShader(_shader);
    m_easyShader->SetOnShaderBind([=](aie::ShaderProgram* s) {
        BindDefaultNormalMapShaderUniforms(s);
        });

    m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, EasyShader* _shader) : m_transform(_transform), m_mesh(_mesh), m_easyShader(_shader)
{
}

Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, aie::OBJMesh* _mesh, EasyShader* _shader) : m_mesh(_mesh), m_easyShader(_shader)
{
    m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

void Instance::Draw()
{
    if (!m_isActive)
        return;

    m_easyShader->BindShader();
    
    m_mesh->draw();
}

void Instance::BindDefaultNormalMapShaderUniforms(aie::ShaderProgram* shader)
{
    // bind the transforms and other relevant uniforms
    auto pvm = m_scene->GetCamera()->GetProjectionMatrix(
        m_scene->GetWindowSize().x,
        m_scene->GetWindowSize().y
    ) * m_scene->GetCamera()->GetViewMatrix() * m_transform;
    shader->bindUniform("ProjectionViewModel", pvm);
    shader->bindUniform("ModelMatrix", m_transform);

    // bind the lighting and camera uniforms
    shader->bindUniform("AmbientColour", m_scene->GetAmbientLight());
    shader->bindUniform("LightColour", m_scene->GetGlobalLight().colour);
    shader->bindUniform("LightDirection", m_scene->GetGlobalLight().direction);

    shader->bindUniform("cameraPosition", m_scene->GetCamera()->GetPosition());

    int numberOfLights = m_scene->GetLightCount();
    shader->bindUniform("numLights", numberOfLights);
    shader->bindUniform("PointLightPositions", numberOfLights, m_scene->GetPointLightPositions());
    shader->bindUniform("PointLightColors", numberOfLights, m_scene->GetPointLightColours());
}

glm::mat4 Instance::MakeTransform(glm::vec3 _positon, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
    return glm::translate(glm::mat4(1), _positon)
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), _scale);
}