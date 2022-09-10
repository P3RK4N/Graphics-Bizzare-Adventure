#pragma once
#include "Engine/Core/Model.h"

#include <windows.h>
#include <unordered_map>

struct aiMaterial;

namespace Engine
{
	enum TextureType
	{
		TextureTypeDiffuse = 0,
		TextureTypeSpecularMap,
		TextureTypeAmbient,
		TextureTypeEmissive,
		TextureTypeHeightmap,
		TextureTypeNormalMap,
		TextureTypeSpecularPowerMap,
		TextureTypeDisplacementMap,
		TextureTypeLightMap,
		TextureTypeEnd
	};

	class ModelMaterial
	{
		friend class Model;

	public:
		ModelMaterial(Model& model);
		~ModelMaterial();

		Model& getModel() { return m_Model; }
		const std::string& getName() const { return m_Name; }
		const std::unordered_map<TextureType, std::vector<std::wstring>*> getTextures() const { return m_Textures; }

	private:
		static void InitializeTextureTypeMappings();
		static std::unordered_map<TextureType, UINT> s_TextureTypeMappings;

		Model& m_Model;
		std::string m_Name{};
		std::unordered_map<TextureType, std::vector<std::wstring>*> m_Textures{};

		ModelMaterial(Model& model, aiMaterial* material);
		ModelMaterial(const ModelMaterial&) = default;
		ModelMaterial& operator=(const ModelMaterial&) = default;
	};
}