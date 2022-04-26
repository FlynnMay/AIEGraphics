#pragma once
#include <glm/ext.hpp>
struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;

	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Initialise(unsigned int _maxParticles, unsigned int _emitRate, float _lifetimeMin, float _lifetimeMax, float _velocityMin, float _velcocityMax, float _startSize, float _endSize, const glm::vec4& _startColour, const glm::vec4 _endColour);

	void Emit();

	void Update(float _dt, const glm::mat4& _cameraTransform);

	void Draw();

	float GetEmitRate() { return m_emitRate; }
	void SetEmitRate(float _rate) { m_emitRate = glm::clamp( _rate, 0.0001f, 1000.0f); }

	float GetLifeSpanMax() { return m_lifespanMax; }
	void SetLifeSpanMax(float _max) { m_lifespanMax = _max; }

	float GetLifeSpanMin() { return m_lifespanMin; }
	void SetLifeSpanMin(float _min) { m_lifespanMin = _min; }
	
	float GetStartSize() { return m_startSize; }
	void SetStartSize(float _size) { m_startSize = _size; }

	float GetEndSize() { return m_endSize; }
	void SetEndSize(float _size) { m_endSize = _size; }

	glm::vec4 GetStartColour() { return m_startColour; }
	void SetStartColour(glm::vec4 _colour) { m_startColour = _colour; }

	glm::vec4 GetEndColour() { return m_endColour; }
	void SetEndColour(glm::vec4 _colour) { m_endColour = _colour; }

	glm::vec3 GetPosition() { return m_position; }
	void SetPosition(glm::vec3 _position) { m_position = _position; }


protected:
	Particle* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_vao;
	unsigned int m_vbo;
	unsigned int m_ibo;
	ParticleVertex* m_vertexData;

	glm::vec3 m_position;

	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;
};

