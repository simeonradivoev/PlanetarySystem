#pragma once
#include "obj_loader.h"
#include "transform.h"
#include "camera.h"
#include <glm\glm.hpp>
#include <GL\glew.h>

typedef struct Vertex
{
public:
	Vertex(){

	}

	Vertex(const glm::vec3& pos,const glm::vec2 uv,const glm::vec3 normal = glm::vec3(0,0,0)){
		this->m_pos = pos;
		this->m_uv = uv;
		this->m_normal = normal;
	}

	glm::vec3* GetPos(){ return &m_pos; }
	glm::vec2* GetUV(){ return &m_uv; }
	glm::vec3* GetNormal(){ return &m_normal; }
	void SetNormal(glm::vec3 normal){ m_normal = normal; }
protected:
private:
	glm::vec3 m_pos;
	glm::vec2 m_uv;
	glm::vec3 m_normal;
}VERTICES;

class Mesh
{
public:
	Mesh(const std::string& fileName);
	Mesh(VERTICES* vertices, unsigned int numVertex, unsigned int* indices, unsigned int numTriangles);
	void Draw();
	virtual ~Mesh();
private:
	void InitMesh(const IndexedModel& model);

	enum MyEnum
	{
		POSITION_VB,
		TEXCOORDS_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_vertexArrayObject;
	GLuint m_vertexArrayBuffers[NUM_BUFFERS];
	unsigned int m_drawCount;
};

