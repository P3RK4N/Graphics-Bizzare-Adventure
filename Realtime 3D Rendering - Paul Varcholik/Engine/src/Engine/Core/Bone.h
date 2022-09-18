#pragma once
#include "SceneNode.h"

#include <windows.h>

namespace Engine
{
	class Bone : public SceneNode
	{
		RTTI_DECLARATIONS(Bone, SceneNode)

	public:
		UINT getIndex() const { return m_Index; }
		void setIndex(UINT index) { m_Index = index; }

		const XMFLOAT4X4& getOffsetTransform() const { return m_OffsetTransform; }
		const XMMATRIX& getOffsetTransformMatrix() const { return XMLoadFloat4x4(&m_OffsetTransform); }

		Bone(const std::string name, UINT index, const XMFLOAT4X4& offsetTransform)
			: SceneNode(name), m_Index(index), m_OffsetTransform(offsetTransform) {}

	private:
		Bone();
		Bone(const Bone&) = default;
		Bone& operator=(const Bone&) = default;

		UINT m_Index = 0;
		XMFLOAT4X4 m_OffsetTransform{};
	};
}