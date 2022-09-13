#pragma once
#include "Material.h"

namespace Engine
{
	struct ProjectionMaterialVertex
	{
		XMFLOAT4 position;
		XMFLOAT2 texturecoordinate;
		XMFLOAT3 normal;

		ProjectionMaterialVertex(const XMFLOAT4& position, const XMFLOAT2& texcoord, const XMFLOAT3& normal)
			: position(position), normal(normal), texturecoordinate(texcoord) {}
		ProjectionMaterialVertex(const ProjectionMaterialVertex&) = default;
		ProjectionMaterialVertex& operator=(const ProjectionMaterialVertex&) = default;
	};

	class ProjectionMaterial : public Material
	{
		RTTI_DECLARATIONS(ProjectionMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(WorldLightViewProjectionScale)
		MATERIAL_VARIABLE_DECLARATION(WorldLightViewProjection)
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
		MATERIAL_VARIABLE_DECLARATION(ProjectedTexture)

	public:
		ProjectionMaterial()
			: Material("project_texture") {}

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void createVertexBuffer(ID3D11Device* device, ProjectionMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT getVertexSize() const override { return sizeof(ProjectionMaterialVertex); }
	};

}