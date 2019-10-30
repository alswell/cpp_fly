#pragma once
#include "env.h"
#include "vector3.h"


template <class T>
class C3DObj
{
	//protected:
public:
	list<CVector3<T>*> m_lsPts;
	//void AddPoint(const CPoint3D<T>& pt)
	//{
	//	m_lsPts.push_back(new CPoint3D<T>(pt));
	//}

	void Move(const CVector3<T>& delta)
	{
		FOR_EACH(it, m_lsPts)
			(**it) += delta;
	}

	void Rotation3DX(const CVector3<T>& pt0, T delta)
	{
		FOR_EACH(it, m_lsPts)
			**it = (**it).Rotation3DX(pt0, delta);
	}
	void Rotation3DY(const CVector3<T>& pt0, T delta)
	{
		FOR_EACH(it, m_lsPts)
			**it = (**it).Rotation3DY(pt0, delta);
	}
	void Rotation3DZ(const CVector3<T>& pt0, T delta)
	{
		FOR_EACH(it, m_lsPts)
			**it = (**it).Rotation3DZ(pt0, delta);
	}

	void ZoomIn(float n)
	{
		FOR_EACH(it, m_lsPts)
			(**it) *= n;
	}
	void ZoomOut(float n)
	{
		FOR_EACH(it, m_lsPts)
			(**it) /= n;
	}
};
