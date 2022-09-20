#pragma once
#include "Material.h"

namespace Engine
{
	class ComputeMaterial : public Material
	{
		RTTI_DECLARATIONS(ComputeMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(TextureSize)
		MATERIAL_VARIABLE_DECLARATION(BlueColor)
		MATERIAL_VARIABLE_DECLARATION(OutputTexture)		

	public:
		ComputeMaterial();

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override {}
		virtual UINT getVertexSize() const override { return 0; }
	};
}