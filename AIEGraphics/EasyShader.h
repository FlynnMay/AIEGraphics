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
	EasyShader() {}
	EasyShader(aie::ShaderProgram* _shader) : m_shader(_shader) {}
	~EasyShader() {}

	void SetOnShaderUse(std::function<void(aie::ShaderProgram* _senderShader)> _func) { m_onShaderUseFunc = _func; }
	void OnShaderUse() { m_onShaderUseFunc(m_shader); }
	void SetOnShaderBind(std::function<void(aie::ShaderProgram* _senderShader)> _func) { m_onBindShader = _func; }
	void OnBindShader() 
	{
		m_shader->bind();
		m_onBindShader(m_shader); 
	}

	aie::ShaderProgram* GetShader() { return m_shader; }
	void SetShader(aie::ShaderProgram* _shader) { m_shader = _shader; }

protected:
	aie::ShaderProgram* m_shader;
	std::function<void(aie::ShaderProgram* _senderShader)> m_onShaderUseFunc;
	std::function<void(aie::ShaderProgram* _senderShader)> m_onBindShader;
};

