#pragma once

#include "Shader.h"
#include <functional>

namespace aie
{
	class ShaderProgram;
}

class EasyShader
{
public:
	EasyShader() : m_shader(nullptr) {}
	EasyShader(aie::ShaderProgram* _shader) : m_shader(_shader) {}
	~EasyShader() {}

	void SetOnShaderUse(std::function<void(aie::ShaderProgram* _senderShader)> _func) { m_onShaderUseFunc = _func; }
	void OnShaderUse() { m_onShaderUseFunc(m_shader); }

	// used to bind uniforms to a shader, called in the 'BindShader' function 
	void SetOnShaderBind(std::function<void(aie::ShaderProgram* _senderShader)> _func) { m_onBindShader = _func; }
	
	// call in a draw function to bind uniforms to the shader
	void BindShader() 
	{
		m_shader->bind();
		m_onBindShader(m_shader); 
	}

	// returns the attatched shader
	aie::ShaderProgram* GetShader() { return m_shader; }

	// sets the attatched shader
	void SetShader(aie::ShaderProgram* _shader) { m_shader = _shader; }

protected:
	aie::ShaderProgram* m_shader;
	std::function<void(aie::ShaderProgram* _senderShader)> m_onShaderUseFunc;
	std::function<void(aie::ShaderProgram* _senderShader)> m_onBindShader;
};

