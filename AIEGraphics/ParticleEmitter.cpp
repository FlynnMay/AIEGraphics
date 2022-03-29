#include "ParticleEmitter.h"
#include <gl_core_4_4.h>


ParticleEmitter::ParticleEmitter() : m_particles(nullptr), m_firstDead(0), m_maxParticles(0), m_position(0, 0, 0), m_vao(0), m_vbo(0), m_ibo(0), m_vertexData(nullptr)
{
}

ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;

	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void ParticleEmitter::Initialise(unsigned int _maxParticles, unsigned int _emitRate, float _lifetimeMin,
	float _lifetimeMax, float _velocityMin, float _velcocityMax, float _startSize, float _endSize,
	const glm::vec4& _startColour, const glm::vec4 _endColour)
{
	// set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / _emitRate;

	// store all variables passed in
	m_startColour = _startColour;
	m_endColour = _endColour;
	m_startSize = _startSize;
	m_endSize = _endSize;
	m_velocityMin = _velocityMin;
	m_velocityMax = _velcocityMax;
	m_lifespanMin = _lifetimeMin;
	m_lifespanMax = _lifetimeMax;
	m_maxParticles = _maxParticles;

	// create particle array
	m_particles = new Particle[m_maxParticles];
	m_firstDead = 0;

	// created the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update.
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// create the index buffer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; i++)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;

		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 * sizeof(ParticleVertex), m_vertexData, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 * sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;
}

void ParticleEmitter::Emit()
{
	// only emmit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	// ressurect the first dead particle
	Particle& particle = m_particles[m_firstDead++];

	// assign its starting position
	particle.position = m_position;

	// randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) * (m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// set the starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// randomise velocity, directiona and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;
}

void ParticleEmitter::Update(float _dt, const glm::mat4& _cameraTransform)
{
	// spawn particles
	m_emitTimer += _dt;
	while (m_emitTimer > m_emitRate)
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}

	unsigned int quad = 0;

	// update the particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_firstDead; i++)
	{
		Particle* particle = &m_particles[i];
		particle->lifetime += _dt;
		if (particle->lifetime >= particle->lifespan)
		{
			// swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			// move particle
			particle->position += particle->velocity * _dt;
			// size particle
			particle->size = glm::mix(m_startSize, m_endSize, particle->lifetime / particle->lifespan);
			// colour particle
			particle->colour = glm::mix(m_startColour, m_endColour, particle->lifetime / particle->lifespan);

			// make a quad the correct size and colour
			float halfsize = particle->size * 0.5f;
			m_vertexData[quad * 4 + 0].position = glm::vec4(halfsize, halfsize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position = glm::vec4(-halfsize, halfsize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;
			m_vertexData[quad * 4 + 2].position = glm::vec4(-halfsize, -halfsize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;
			m_vertexData[quad * 4 + 3].position = glm::vec4(halfsize, -halfsize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			// create billboard transform
			glm::vec3 zAxis = glm::normalize(glm::vec3(_cameraTransform[3]) - particle->position);
			glm::vec3 xAxis = glm::cross(glm::vec3(_cameraTransform[1]), zAxis);
			glm::vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(
				glm::vec4(xAxis, 0),
				glm::vec4(yAxis, 0),
				glm::vec4(zAxis, 0),
				glm::vec4(0, 0, 0, 1));


			m_vertexData[quad * 4 + 0].position = billboard *
				m_vertexData[quad * 4 + 0].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 1].position = billboard *
				m_vertexData[quad * 4 + 1].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 2].position = billboard *
				m_vertexData[quad * 4 + 2].position +
				glm::vec4(particle->position, 0);

			m_vertexData[quad * 4 + 3].position = billboard *
				m_vertexData[quad * 4 + 3].position +
				glm::vec4(particle->position, 0);

			++quad;
		}
	}
}

void ParticleEmitter::Draw()
{
	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 * sizeof(ParticleVertex), m_vertexData);

	// draw particles
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}
