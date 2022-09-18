#pragma once
#include "RTTI.h"

#include <vector>

#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class SceneNode : public RTTI
	{
		RTTI_DECLARATIONS(SceneNode, RTTI)

	public:
		const std::string& getName() const { return m_Name; }
		SceneNode* getParent() { return m_Parent; }
		std::vector<SceneNode*>& getChildren() { return m_Children; }
		const XMFLOAT4X4& getTransform() const { return m_Transform; }
		const XMMATRIX& getTransformMatrix() const { return XMLoadFloat4x4(&m_Transform); }

		void setParent(SceneNode* parent) { m_Parent = parent; }
		void setTransform(const XMFLOAT4X4& transform) { m_Transform = transform; }
		void setTransform(const CXMMATRIX& transform) { XMStoreFloat4x4(&m_Transform, transform); }

		SceneNode(const std::string& name)
			: m_Name(name) {}
		SceneNode(const std::string& name, const XMFLOAT4X4& transform)
			: m_Name(name), m_Transform(transform) {}

	protected:
		std::string m_Name{};
		SceneNode* m_Parent = nullptr;
		std::vector<SceneNode*> m_Children{};
		XMFLOAT4X4 m_Transform{};

	private:
		SceneNode();
		SceneNode(const SceneNode&) = default;
		SceneNode& operator=(const SceneNode&) = default;
	};
}