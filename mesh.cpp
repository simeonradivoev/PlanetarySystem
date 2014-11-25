#include "mesh.h"
#include <glm\gtc\type_ptr.hpp>
#include <vector>

Mesh::Mesh(VERTICES* vertices, unsigned int numVertex, unsigned int* indices, unsigned int numTriangles)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertex; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.uvs.push_back(*vertices[i].GetUV());
		model.normals.push_back(*vertices[i].GetNormal());
	}

	for (unsigned int i = 0; i < numTriangles; i++)
	{
		model.indices.push_back(indices[i]);
	}

	InitMesh(model);
}

Mesh::Mesh(const std::string& fileName)
{
	IndexedModel model = OBJModel(fileName).ToIndexedModel();
	InitMesh(model);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1,&m_vertexArrayObject);
}

void Mesh::InitMesh(const IndexedModel& model)
{
	m_drawCount = model.indices.size();

	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

	#pragma region Positions
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	//position | num of Elements
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	#pragma endregion

	#pragma region UVs
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORDS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.uvs[0]) * model.uvs.size(), &model.uvs[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	#pragma endregion

	#pragma region Normals
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	#pragma endregion

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);


	glBindVertexArray(0);
}

void Mesh::Draw()
{
	glBindVertexArray(m_vertexArrayObject);

	glDrawElements(GL_TRIANGLES,m_drawCount,GL_UNSIGNED_INT,0);

	glBindVertexArray(0);
}