#include "pch.h"
#include "Technique.h"

namespace Engine
{
	Technique::Technique(Application* app, Effect* effect, ID3DX11EffectTechnique* technique)
		: m_Effect(effect), m_Technique(technique)
	{
		m_Technique->GetDesc(&m_TechniqueDesc);
		m_Name = m_TechniqueDesc.Name;

		for(UINT i = 0; i < m_TechniqueDesc.Passes; i++)
		{
			Pass* pass = new Pass(app, this, m_Technique->GetPassByIndex(i));
			m_Passes.push_back(pass);
			m_PassesByName.insert(std::pair<std::string, Pass*>(pass->getName(), pass));
		}
	}

	Technique::~Technique()
	{
		for(auto& pass : m_Passes)
			DeleteObject(pass);

		ReleaseObject(m_Technique);
	}
	
}
