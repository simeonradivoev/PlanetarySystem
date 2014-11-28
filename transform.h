#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm\detail\precision.hpp>
using namespace glm;

class Transform
{
public:
	Transform(const dvec3& pos = dvec3(), const quat& rot = quat(), const dvec3& scale = dvec3(1.0, 1.0, 1.0)) :
		m_pos(pos), m_rot(rot), m_scale(scale){}

	inline dmat4 GetModel() const
	{
		dmat4 posMatrix = translate(m_pos);
		dmat4 scaleMatrix = glm::scale(m_scale);
		dmat4 rotMatrix = mat4_cast(m_rot);

		return posMatrix * rotMatrix * scaleMatrix;
	}

	_declspec(property(get = GetPos, put = SetPos)) dvec3& position;
	_declspec(property(get = GetRot, put = SetRot)) dquat& rotation;
	_declspec(property(get = GetScale, put = SetScale)) dvec3& scale;

	inline dvec3& GetPos(){ return m_pos; }
	inline dquat& GetRot(){ return m_rot; }
	inline dvec3& GetScale(){ return m_scale; }
	inline dvec3& GetForward(){ return axis(m_rot); }

	inline dmat4 GetTranslationMatrix(){ return translate(m_pos); } const
	inline dmat4 GetRotationMatrix(){ return mat4_cast(m_rot); } const
	inline dmat4 GetScaleMatrix(){ return glm::scale(m_scale); } const

	inline void SetPos(const dvec3 pos){ m_pos = pos; }
	inline void SetRot(const dquat rot){ m_rot = rot; }
	inline void AngleAxis(const double angle, const glm::dvec3 axis){ m_rot = angleAxis(angle, axis); }
	inline void SetScale(const dvec3 scale){ m_scale = scale; }
	virtual ~Transform(){}
private:
	dvec3 m_pos;
	dquat m_rot;
	dvec3 m_scale;
};
#endif //TRANSFORM_H