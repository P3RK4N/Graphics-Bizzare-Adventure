#pragma once
#include "Light.h"

namespace Engine
{
	class DirectionalLight : public Light
	{
		RTTI_DECLARATIONS(DirectionalLight, Light)

	public:
		DirectionalLight(const XMCOLOR& color, const XMFLOAT3& direction)
			: Light(color), m_Direction(direction) {}
		virtual ~DirectionalLight() {}

		void applyRotation(const XMVECTOR& quaternion);
		const XMFLOAT3& getDirection() const { return m_Direction; }
		const XMVECTOR& getDirectionVector() const { return XMLoadFloat3(&m_Direction); }

	private:
		XMFLOAT3 m_Direction;
	};
}