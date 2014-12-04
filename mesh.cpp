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

Mesh* Mesh::Sphere(const double radius, const int segments)
{
	double angle = 0;
	const unsigned int VertexCount = (180 / segments) * (360 / segments) * 4;
	const unsigned int indexCount = (VertexCount / 2) * 3;
	VERTICES* vertex = new VERTICES[VertexCount];
	unsigned int* indices = new unsigned int[indexCount];
	//current vertex
	int n;
	//current index
	int i;
	//loop controls
	double a, b;
	double H = 0, K = 0, Z = 0;

	n = 0;
	i = 0;

	for (b = 0; b <= 180 - segments; b += segments){
		for (a = 0; a <= 360 - segments; a += segments){
			vertex[n].GetPos()->x = radius * sin(glm::radians(a)) * sin(glm::radians(b)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a)) * sin(glm::radians(b)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b)) - Z;

			vertex[n].GetUV()->y = (b) / 180.0;
			vertex[n].GetUV()->x = (a) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//b + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a)) * sin(glm::radians(b + segments)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a)) * sin(glm::radians(b + segments)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b + segments)) - Z;

			vertex[n].GetUV()->y = (b + segments) / 180.0;
			vertex[n].GetUV()->x = (a) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//a + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a + segments)) * sin(glm::radians(b)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a + segments)) * sin(glm::radians(b)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b)) - Z;

			vertex[n].GetUV()->y = (b) / 180.0;
			vertex[n].GetUV()->x = (a + segments) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			//a + b + segments
			vertex[n].GetPos()->x = radius * sin(glm::radians(a + segments)) * sin(glm::radians(b + segments)) - H;
			vertex[n].GetPos()->z = radius * cos(glm::radians(a + segments)) * sin(glm::radians(b + segments)) + K;
			vertex[n].GetPos()->y = radius * cos(glm::radians(b + segments)) - Z;

			vertex[n].GetUV()->y = (b + segments) / 180.0;
			vertex[n].GetUV()->x = (a + segments) / 360.0;
			vertex[n].SetNormal(glm::normalize(*vertex[n].GetPos()));
			n++;

			indices[i++] = n - 4;
			indices[i++] = n - 3;
			indices[i++] = n - 1;

			indices[i++] = n - 4;
			indices[i++] = n - 1;
			indices[i++] = n - 2;


		}
	}

	return new Mesh(vertex, VertexCount, indices,indexCount);
}

Mesh* Mesh::Plane(const float size)
{
	VERTICES vertex[]
	{
		Vertex(glm::vec3(-1, 0, -1) * size, glm::vec2(0, 0), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(-1, 0, 1) * size, glm::vec2(0, 1), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(1, 0, 1) * size, glm::vec2(1, 1), glm::vec3(0, 1, 0)),
			Vertex(glm::vec3(1, 0, -1) * size, glm::vec2(1, 0), glm::vec3(0, 1, 0))
	};

	unsigned int indecies[]
	{
		0, 1, 2,
			0, 2, 3
	};

	return new Mesh(vertex,4,indecies,6);
}