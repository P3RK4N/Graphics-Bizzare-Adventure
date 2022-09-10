#include "pch.h"
#include "Material.h"

#include "Engine/Core/Model.h"

namespace Engine
{
	RTTI_DEFINITIONS(Material)

	Material::~Material()
	{
		for(std::pair<Pass*, ID3D11InputLayout*> inputLayout : m_InputLayouts)
			ReleaseObject(inputLayout.second);
	}

	Variable* Material::operator[](const std::string& variableName)
	{
		const std::unordered_map<std::string, Variable*>& variables = m_Effect->getVariablesByName();
		Variable* foundVariable = nullptr;

		std::unordered_map<std::string, Variable*>::const_iterator found = variables.find(variableName);
		if(found != variables.end()) foundVariable = found->second;

		return foundVariable;
	}

	void Material::initialize(Effect* effect)
	{
		m_Effect = effect;
		assert(m_Effect);

		Technique* defaultTechnique = nullptr;
		if(!m_DefaultTechniqueName.empty())
		{
			defaultTechnique = m_Effect->getTechniquesByName().at(m_DefaultTechniqueName);
			assert(defaultTechnique);
		}
		else
		{
			defaultTechnique = m_Effect->getTechniques().at(0);
		}

		m_CurrentTechnique = defaultTechnique;
	}

	void Material::createVertexBuffer(ID3D11Device* device, const Model& model, std::vector<ID3D11Buffer*>& vertexBuffers) const
	{
		vertexBuffers.reserve(model.getMeshes().size());

		for(Mesh* mesh : model.getMeshes())
		{
			ID3D11Buffer* vertexBuffer;
			createVertexBuffer(device, *mesh, &vertexBuffer);
			vertexBuffers.push_back(vertexBuffer);
		}
	}

	void Material::createInputLayout(const std::string& techniqueName, const std::string& passName, D3D11_INPUT_ELEMENT_DESC* inputElementDescriptions, UINT inputElementDescriptionCount)
	{
		Technique* technique = m_Effect->getTechniquesByName().at(techniqueName);
		assert(technique);

		Pass* pass = technique->getPassesByName().at(passName);
		assert(pass);

		ID3D11InputLayout* inputLayout;
		pass->createInputLayout(inputElementDescriptions, inputElementDescriptionCount, &inputLayout);
		m_InputLayouts.insert(std::pair<Pass*, ID3D11InputLayout*>(pass, inputLayout));
	}


}
