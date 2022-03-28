#include <gl_core_4_4.h>
#include "Mesh.h"

Mesh::~Mesh()
{
	// If the array does not equal zero, then delete the data
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::InitialiseQuad()
{
	// Check if the mesh is not initialised already
	assert(m_vao == 0);

	// Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);
	
	// Bind the vertex array, this is our mesh wrapper
	glBindVertexArray(m_vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Define 6 vertices for our two triangles
	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0.0f,  0.5f, 1.0f };
	vertices[1].position = {  0.5f, 0.0f,  0.5f, 1.0f };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1.0f };
	vertices[3].position = { -0.5f, 0.0f,  -0.5f, 1.0f };
	vertices[4].position = {  0.5f, 0.0f,  0.5f, 1.0f };
	vertices[5].position = {  0.5f, 0.0f, -0.5f, 1.0f };

	vertices[0].normal = { 0, 1, 0, 0 };
	vertices[1].normal = { 0, 1, 0, 0 };
	vertices[2].normal = { 0, 1, 0, 0 };
	vertices[3].normal = { 0, 1, 0, 0 };
	vertices[4].normal = { 0, 1, 0, 0 };
	vertices[5].normal = { 0, 1, 0, 0 };

	vertices[0].texCoord = { 0, 1 }; // bottom left
	vertices[1].texCoord = { 1, 1 }; // bottom right
	vertices[2].texCoord = { 0, 0 }; // top left
	vertices[3].texCoord = { 0, 0 }; // top left
	vertices[4].texCoord = { 1, 1 }; // bottom right
	vertices[5].texCoord = { 1, 0 }; // top right


	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// Enable Second element as the normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	// Enable the third element as the texture coord
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)32);

	// Unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// This quad is made of 2 triangles
	m_triCount = 2;
}

void Mesh::InitialiseFullScreenQuad()
{
	assert(m_vao == 0);

	// Generate Buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	//Then we bind the vertex array into a mesh wrapper
	glBindVertexArray(m_vao);

	// Then bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Degine the quads vertices
	float vertices[] = {
		-1, 1, // left, top
		-1,-1, // left, bottom
		1,1,   // right, top

		-1,-1, // left, bottom
		1,-1,   // right, bottom
		1,1   // right, top
	};

	// Then fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, GL_STATIC_DRAW);

	// Now enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, 0);

	// Unbind the buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// This quad was made with 2 triangles
	m_triCount = 2;
}

void Mesh::Initialise(unsigned int vertexCount, const Vertex* vertices, unsigned int indexCount, unsigned int* indices)
{
	// Check if the mesh is not initialised already
	assert(m_vao == 0);

	// Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	// Bind the vertex array, this is our mesh wrapper
	glBindVertexArray(m_vao);

	// Bind the vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Fill the vertex buffer
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// Enable the first element as the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	if (indexCount != 0) 
	{
		glGenBuffers(1, &m_ibo);

		//bind the vertex
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		// fill the vertex buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		m_triCount = indexCount / 3;
	}
	else 
	{
		m_triCount = vertexCount / 3;
	}

	// Unbind Buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::CreateCube()
{
	// Define 8 vertices for our two triangles
	Mesh::Vertex vertices[8];
	vertices[0].position = { -0.5f, -0.5f, -0.5f, 1.0f };
	vertices[1].position = { -0.5f, 0.5f, -0.5f, 1.0f };
	vertices[2].position = { 0.5f, 0.5f, -0.5f, 1.0f };
	vertices[3].position = { 0.5f, -0.5f , -0.5f, 1.0f };

	vertices[4].position = { -0.5f, -0.5f, 0.5f, 1.0f };
	vertices[5].position = { -0.5f, 0.5f, 0.5f, 1.0f };
	vertices[6].position = { 0.5f, 0.5f, 0.5f, 1.0f };
	vertices[7].position = { 0.5f, -0.5f, 0.5f, 1.0f };

	unsigned int indices[36] = { 
		0,1,3,1,2,3,
		3,2,7,2,6,7,
		6,5,7,7,5,4,
		4,1,0,4,5,1,
		0,3,4,4,3,7,
		5,2,1,6,2,5,
	};

	Initialise(8, vertices, 36, indices);
}

void Mesh::CreatePyramid()
{
	// Define 5 vertices for our two triangles
	Mesh::Vertex vertices[5];
	vertices[0].position = { 0.0f, 1.0f, 0.0f, 1.0f };
	vertices[1].position = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertices[2].position = { 1.0f, -1.0f, -1.0f, 1.0f };
	vertices[3].position = { 1.0f, -1.0f, 1.0f, 1.0f };
	vertices[4].position = { -1.0f, -1.0f , 1.0f, 1.0f };

	unsigned int indices[18] = { 
		1,2,4,4,2,3,
		4,0,1,
		3,0,4,
		2,0,3,
		1,0,2,
	};

	Initialise(8, vertices, 36, indices);
}

void Mesh::CreateGrid(int cols, int rows)
{
	// Define 5 vertices for our two triangles
	Mesh::Vertex* vertices = new Mesh::Vertex[cols*rows];
	unsigned int* indices = new unsigned int[cols * rows];

	Initialise(8, vertices, 36, indices);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vao);

	// Are we actually using the indices or just the vertices
	if (m_ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * m_triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_triCount);
}
