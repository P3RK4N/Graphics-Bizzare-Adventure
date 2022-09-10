#pragma once
#include "Technique.h"

#include "Engine/Core/Variable.h"

#include <string>
#include <vector>
#include <unordered_map>

#include <d3dx11effect.h>

namespace Engine
{
	class Application;

	class Effect
	{
	public:
		Effect(Application* app)
			: m_Application(app) {}
		
		virtual ~Effect();

		static void compileEffectFromFile(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& fileName);
		static void loadCompiledEffect(ID3D11Device* device, ID3DX11Effect** effect, const std::wstring& fileName);

		Application* getApplication() { return m_Application; }
		ID3DX11Effect* getEffect() const { return m_Effect; }
		const D3DX11_EFFECT_DESC& getEffectDesc() const { return m_EffectDesc; }
		const std::vector<Technique*>& getTechniques() const { return m_Techniques; }
		const std::unordered_map<std::string, Technique*> getTechniquesByName() { return m_TechniquesByName; }
		const std::vector<Variable*>& getVariables() const { return m_Variables; }
		const std::unordered_map<std::string, Variable*>& getVariablesByName() const { return m_VariablesByName; }

		void compileFromFile(const std::wstring& fileName);
		void loadCompiledEffect(const std::wstring& fileName);

	private:
		Effect(const Effect&) = default;
		Effect& operator=(const Effect&) = default;

		void initialize();

		Application* m_Application = nullptr;
		ID3DX11Effect* m_Effect = nullptr;
		D3DX11_EFFECT_DESC m_EffectDesc{};
		std::vector<Technique*> m_Techniques{};
		std::unordered_map<std::string, Technique*> m_TechniquesByName{};
		std::vector<Variable*> m_Variables{};
		std::unordered_map<std::string, Variable*> m_VariablesByName{};
	};
}