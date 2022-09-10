#pragma once
#include "Pass.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <d3dx11effect.h>

namespace Engine
{
	class Application;
	class Effect;

	class Technique
	{
	public:
		Technique(Application* app, Effect* effect, ID3DX11EffectTechnique* technique);
		~Technique();

		Effect* getEffect() { return m_Effect; }
		ID3DX11EffectTechnique* getTechnique() const { return m_Technique; }
		const D3DX11_TECHNIQUE_DESC& getTechniqueDesc() const { return m_TechniqueDesc; }
		const std::string& getName() const { return m_Name; }
		const std::vector<Pass*>& getPasses() const { return m_Passes; }
		const std::unordered_map<std::string, Pass*> getPassesByName() const { return m_PassesByName; }

	private:
		Technique(const Technique&) = default;
		Technique& operator=(const Technique&) = default;

		Effect* m_Effect = nullptr;
		ID3DX11EffectTechnique* m_Technique = nullptr;
		D3DX11_TECHNIQUE_DESC m_TechniqueDesc{};
		std::string m_Name{};
		std::vector<Pass*> m_Passes{};
		std::unordered_map<std::string, Pass*> m_PassesByName{};
	};		
}