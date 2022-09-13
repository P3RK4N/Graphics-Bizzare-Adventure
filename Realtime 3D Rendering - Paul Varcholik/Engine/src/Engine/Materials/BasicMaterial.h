#pragma once
#include "Material.h"

namespace Engine
{
	struct BasicMaterialVertex
	{
		XMFLOAT4 position;
		XMFLOAT4 color;

		BasicMaterialVertex() = default;
		BasicMaterialVertex(const BasicMaterialVertex&) = default;
		BasicMaterialVertex(const XMFLOAT4& position, const XMFLOAT4& color)
			: position(position), color(color) {}
		BasicMaterialVertex& operator=(const BasicMaterialVertex&) = default;
	};

	class BasicMaterial : public Material
	{
		RTTI_DECLARATIONS(BasicMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)

	public:
		BasicMaterial();

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void createVertexBuffer(ID3D11Device* device, BasicMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT getVertexSize() const override;

	};
}