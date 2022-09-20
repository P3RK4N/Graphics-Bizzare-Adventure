#include "pch.h"
#include "PostProcessMaterial.h"

#include "Engine/Core/Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(PostProcessMaterial)

	MATERIAL_VARIABLE_DEFINITION(PostProcessMaterial, ColorTexture)
	MATERIAL_VARIABLE_DEFINITION(PostProcessMaterial, DistortionTexture)


	void PostProcessMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(ColorTexture)
		MATERIAL_VARIABLE_RETRIEVE(DistortionTexture)

		D3D11_INPUT_ELEMENT_DESC inputDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		createInputLayout("filter", "grayscale", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
		createInputLayout("filter", "inverse", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
		createInputLayout("filter", "blur", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
		createInputLayout("filter", "distortion", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
		createInputLayout("filter", "nothing", inputDesc, sizeof(inputDesc) / sizeof(inputDesc[0]));
	}

}
