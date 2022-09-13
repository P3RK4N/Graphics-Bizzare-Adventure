#pragma once
#include "Material.h"

namespace Engine
{
	struct DiffuseLightingMaterialVertex
	{
		XMFLOAT4 position;
		XMFLOAT2 texturecoordinate;
		XMFLOAT3 normal;

		DiffuseLightingMaterialVertex(const XMFLOAT4& position, const XMFLOAT2& texcoord, const XMFLOAT3& normal)
			: position(position), normal(normal), texturecoordinate(texcoord) {}
		DiffuseLightingMaterialVertex(const DiffuseLightingMaterialVertex&) = default;
		DiffuseLightingMaterialVertex& operator=(const DiffuseLightingMaterialVertex&) = default;
	};

	class DiffuseLightingMaterial : public Material
	{
		RTTI_DECLARATIONS(DiffuseLightingMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(World)
		MATERIAL_VARIABLE_DECLARATION(CameraPosition)

		MATERIAL_VARIABLE_DECLARATION(AmbientColor)
		MATERIAL_VARIABLE_DECLARATION(SpecularColor)
		MATERIAL_VARIABLE_DECLARATION(SpecularPower)

		MATERIAL_VARIABLE_DECLARATION(LightColor)
		MATERIAL_VARIABLE_DECLARATION(LightPosition)
		MATERIAL_VARIABLE_DECLARATION(LightDirection)
		MATERIAL_VARIABLE_DECLARATION(LightRadius)

		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

	public:
		DiffuseLightingMaterial()
			: Material("main11") {}

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void createVertexBuffer(ID3D11Device* device, DiffuseLightingMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT getVertexSize() const override { return sizeof(DiffuseLightingMaterialVertex); }
	};

}