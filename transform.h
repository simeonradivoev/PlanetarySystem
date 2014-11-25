#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm\detail\precision.hpp>

class Transform
{
public:
	Transform(const glm::dvec3& pos = glm::dvec3(), const glm::quat& rot = glm::quat(), const glm::dvec3& scale = glm::dvec3(1.0, 1.0, 1.0)) :
		m_pos(pos), m_rot(rot), m_scale(scale){}

	inline glm::dmat4 GetModel() const
	{
		glm::dmat4 posMatrix = glm::translate(m_pos);
		/*mat4 rotXMatrix = rotate(m_rot.x,glm::vec3(1,0,0));
		mat4 rotYMatrix = rotate(m_rot.y, glm::vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rot.z, glm::vec3(0, 0, 1));*/
		glm::dmat4 scaleMatrix = glm::scale(m_scale);
		glm::dmat4 rotMatrix = glm::mat4_cast(m_rot);

		return posMatrix * rotMatrix * scaleMatrix;
	}

	inline glm::dmat4 GetModelRotationFirst() const
	{
		glm::dmat4 posMatrix = glm::translate(m_pos);
		/*mat4 rotXMatrix = rotate(m_rot.x,glm::vec3(1,0,0));
		mat4 rotYMatrix = rotate(m_rot.y, glm::vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rot.z, glm::vec3(0, 0, 1));*/
		glm::dmat4 scaleMatrix = glm::scale(m_scale);
		glm::dmat4 rotMatrix = glm::mat4_cast(m_rot);

		return rotMatrix * posMatrix * scaleMatrix;
	}

	_declspec(property(get = GetPos, put = SetPos)) glm::dvec3& position;
	_declspec(property(get = GetRot, put = SetRot)) glm::dquat& rotation;
	_declspec(property(get = GetScale, put = SetScale)) glm::dvec3& scale;

	inline glm::dvec3& GetPos(){ return m_pos; }
	inline glm::dquat& GetRot(){ return m_rot; }
	inline glm::dvec3& GetScale(){ return m_scale; }
	inline glm::dvec3& GetForward(){ return glm::axis(m_rot); }

	inline void SetPos(const glm::dvec3 pos){ m_pos = pos; }
	inline void SetRot(const glm::dquat rot){ m_rot = rot; }
	inline void AngleAxis(const double angle, const glm::dvec3 axis){ m_rot = glm::angleAxis(angle, axis); }
	inline void SetScale(const glm::dvec3 scale){ m_scale = scale; }
	virtual ~Transform(){}
private:
	glm::dvec3 m_pos;
	glm::dquat m_rot;
	glm::dvec3 m_scale;
};

