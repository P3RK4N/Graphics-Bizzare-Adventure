#include "pch.h"
#include "ModelMaterial.h"

#include <assimp/scene.h>

namespace Engine
{
	namespace Utility
	{
		std::wstring s2ws(const std::string& str)
		{
			int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
			std::wstring wstrTo( size_needed, 0 );
			MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
			return wstrTo;
		}
	}

	std::unordered_map<TextureType, UINT> ModelMaterial::s_TextureTypeMappings{};

	ModelMaterial::ModelMaterial(Model& model)
		: m_Model(model)
	{
		InitializeTextureTypeMappings();
	}

	ModelMaterial::ModelMaterial(Model& model, aiMaterial* material)
		: m_Model(model)
	{
		InitializeTextureTypeMappings();

		aiString name;
		material->Get(AI_MATKEY_NAME, name);
		m_Name = name.C_Str();

		for(TextureType textureType = (TextureType)0; textureType < TextureTypeEnd; textureType = (TextureType)(textureType+1))
		{
			aiTextureType mappedTextureType = (aiTextureType)s_TextureTypeMappings[textureType];

			UINT textureCount = material->GetTextureCount(mappedTextureType);

			if(textureCount > 0)
			{
				std::vector<std::wstring>* textures = new std::vector<std::wstring>();
				m_Textures.insert(std::pair<TextureType, std::vector<std::wstring>*>(textureType, textures));

				textures->reserve(textureCount);
				for(UINT textureIndex = 0; textureIndex < textureCount; textureIndex++)
				{
					aiString path;
					if(material->GetTexture(mappedTextureType, textureIndex, &path) == AI_SUCCESS)
					{
						std::wstring wPath(Utility::s2ws(path.C_Str()));
						textures->push_back(wPath);
					}
				}
			}
		}
	}

	ModelMaterial::~ModelMaterial()
	{
		for(auto& textures : m_Textures)
			DeleteObject(textures.second);
	}

	void ModelMaterial::InitializeTextureTypeMappings()
	{
		if(s_TextureTypeMappings.size() != TextureTypeEnd)
		{
			s_TextureTypeMappings[TextureTypeDiffuse] = aiTextureType_DIFFUSE;
			s_TextureTypeMappings[TextureTypeSpecularMap] = aiTextureType_SPECULAR;
			s_TextureTypeMappings[TextureTypeAmbient] = aiTextureType_AMBIENT;
			s_TextureTypeMappings[TextureTypeHeightmap] = aiTextureType_HEIGHT;
			s_TextureTypeMappings[TextureTypeNormalMap] = aiTextureType_NORMALS;
			s_TextureTypeMappings[TextureTypeSpecularPowerMap] = aiTextureType_SHININESS;
			s_TextureTypeMappings[TextureTypeDisplacementMap] = aiTextureType_DISPLACEMENT;
			s_TextureTypeMappings[TextureTypeLightMap] = aiTextureType_LIGHTMAP;
		}
	}
}