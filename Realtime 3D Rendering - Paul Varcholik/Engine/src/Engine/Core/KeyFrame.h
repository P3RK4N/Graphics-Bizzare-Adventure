#pragma once

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class KeyFrame
	{
		friend class BoneAnimation;

	public:
		float getTime() const { return m_Time; }
		const XMFLOAT3& getTranslation() const { return m_Translation; }
		const XMFLOAT4& getRotationQuaternion() const { return m_Rotation; }
		const XMFLOAT3& getScale() const { return m_Scale; }

		XMVECTOR getTranslationVector() const { return XMLoadFloat3(&m_Translation); }
		XMVECTOR getRotationVector() const { return XMLoadFloat4(&m_Rotation); }
		XMVECTOR getScaleVector() const { return XMLoadFloat3(&m_Scale); }

		XMMATRIX getTransform() const
		{
			XMFLOAT4 rotation{ 0.0f, 0.0f, 0.0f, 0.0f };
			XMVECTOR rotationOrigin = XMLoadFloat4(&rotation);
			return XMMatrixAffineTransformation(getScaleVector(), rotationOrigin, getRotationVector(), getTranslationVector());
		}

	private:
		KeyFrame(float time, const XMFLOAT3& translation, const XMFLOAT4& rotation, const XMFLOAT3& scale)
			: m_Time(time), m_Translation(translation), m_Rotation(rotation), m_Scale(scale)
		{}

		KeyFrame() = default;
		KeyFrame(const KeyFrame&) = default;
		KeyFrame& operator=(const KeyFrame&) = default;

		float m_Time;
		XMFLOAT3 m_Translation;
		XMFLOAT4 m_Rotation;
		XMFLOAT3 m_Scale;
	};
}
