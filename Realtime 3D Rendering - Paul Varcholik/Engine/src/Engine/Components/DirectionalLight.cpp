#include "pch.h"
#include "DirectionalLight.h"


namespace Engine
{
	RTTI_DEFINITIONS(DirectionalLight)

	void DirectionalLight::applyRotation(const XMVECTOR& rotation)
	{
		XMVECTOR dir = XMLoadFloat3(&m_Direction);
		XMVector3Rotate(dir, rotation);
		XMStoreFloat3(&m_Direction, dir);
	}
	

}
