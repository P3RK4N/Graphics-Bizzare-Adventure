#pragma once
#include "Material.h"

namespace Engine
{
	class PostProcessMaterial : public Material
	{
		RTTI_DECLARATIONS(PostProcessMaterial, Material)

		MATERIAL_VARIABLE_DECLARATION(ColorTexture)
		MATERIAL_VARIABLE_DECLARATION(DistortionTexture)

	public:
		PostProcessMaterial()
			: Material("filter") {}

		virtual void initialize(Effect* effect) override;
		virtual void createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const override {}
		virtual UINT getVertexSize() const override { return 0; }

	private:
		
	};

}