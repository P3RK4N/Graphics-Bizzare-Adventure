#include "pch.h"
#include "ComputeMaterial.h"

#include "Engine/Core/Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(ComputeMaterial)

	MATERIAL_VARIABLE_DEFINITION(ComputeMaterial, TextureSize)
	MATERIAL_VARIABLE_DEFINITION(ComputeMaterial, BlueColor)
	MATERIAL_VARIABLE_DEFINITION(ComputeMaterial, OutputTexture)	

	ComputeMaterial::ComputeMaterial() :
		Material("compute") {}

	void ComputeMaterial::initialize(Effect* effect)
	{
		Material::initialize(effect);

		MATERIAL_VARIABLE_RETRIEVE(TextureSize);
		MATERIAL_VARIABLE_RETRIEVE(BlueColor);
		MATERIAL_VARIABLE_RETRIEVE(OutputTexture);

	}
}
