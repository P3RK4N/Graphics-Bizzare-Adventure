#pragma once
#include "Material.h"

namespace Engine
{
	struct AnimationMaterialVertex
	{
		XMFLOAT4 position;
		XMFLOAT2 texturecoordinate;
		XMFLOAT3 normal;
		XMUINT4 indices;
		XMFLOAT4 weights;

		AnimationMaterialVertex(const XMFLOAT4& position, const XMFLOAT2& texcoord, const XMFLOAT3& normal, const XMUINT4& indices, const XMFLOAT4& weights)
			: position(position), normal(normal), texturecoordinate(texcoord), weights(weights), indices(indices) {}
		AnimationMaterialVertex(const AnimationMaterialVertex&) = default;
		AnimationMaterialVertex& operator=(const AnimationMaterialVertex&) = default;
	};

	struct CBBone
	{
		XMFLOAT4X4 boneTransforms[60];
	};

	class AnimationMaterial : public Material
	{
		RTTI_DECLARATIONS(AnimationMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(WorldViewProjection)
		MATERIAL_VARIABLE_DECLARATION(World)
		MATERIAL_VARIABLE_DECLARATION(CameraPosition)

		MATERIAL_VARIABLE_DECLARATION(AmbientColor)
		MATERIAL_VARIABLE_DECLARATION(SpecularColor)
		MATERIAL_VARIABLE_DECLARATION(SpecularPower)

		MATERIAL_VARIABLE_DECLARATION(LightColor)
		MATERIAL_VARIABLE_DECLARATION(LightPosition)
		MATERIAL_VARIABLE_DECLARATION(LightRadius)

		MATERIAL_VARIABLE_DECLARATION(ColorTexture)

		MATERIAL_VARIABLE_DECLARATION(BoneTransforms)

	public:
		AnimationMaterial()
			: Material("main11") {}

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override;
		void createVertexBuffer(ID3D11Device* device, AnimationMaterialVertex* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer) const;
		virtual UINT getVertexSize() const override { return sizeof(AnimationMaterialVertex); }

	};

}