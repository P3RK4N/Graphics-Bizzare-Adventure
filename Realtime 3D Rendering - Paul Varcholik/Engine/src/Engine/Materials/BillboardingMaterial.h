#pragma once
#include "Material.h"

namespace Engine
{
	struct BillboardingMaterialVertex
	{
		XMFLOAT4 position{};
		XMFLOAT2 size{};

		BillboardingMaterialVertex() = default;
		BillboardingMaterialVertex(const XMFLOAT4& position, const XMFLOAT2& size)
			: position(position), size(size){}
		BillboardingMaterialVertex(const BillboardingMaterialVertex&) = default;
		BillboardingMaterialVertex& operator=(const BillboardingMaterialVertex&) = default;
	};

	class BillboardingMaterial : public Material
	{
		RTTI_DECLARATIONS(BillboardingMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(ViewProjection)
		MATERIAL_VARIABLE_DECLARATION(TextureMatrix)
		MATERIAL_VARIABLE_DECLARATION(CameraPosition)
		MATERIAL_VARIABLE_DECLARATION(SubdivisionFactor)
		MATERIAL_VARIABLE_DECLARATION(SubdivisionStrength)
		MATERIAL_VARIABLE_DECLARATION(CameraUp)
		
		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

	public:
		BillboardingMaterial()
			: Material("main11") {}

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void createVertexBuffer(ID3D11Device* device, BillboardingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT getVertexSize() const override { return sizeof(BillboardingMaterialVertex); }
	};

}