#pragma once

#include <string>

#include <d3dx11effect.h>

namespace Engine
{
	class Application;
	class Technique;

	class Pass
	{
	public:
		Pass(Application* app, Technique* technique, ID3DX11EffectPass* pass);
		~Pass();

		Technique* getTechnique() { return m_Technique; }
		ID3DX11EffectPass* getPass() const { return m_Pass; }
		const D3DX11_PASS_DESC& getPassDesc() const { return m_PassDesc; }
		const std::string& getName() const { return m_Name; }

		void createInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements, ID3D11InputLayout** inputLayout);
		void apply(UINT flags, ID3D11DeviceContext* context);

	private:
		Pass(const Pass&) = default;
		Pass& operator=(const Pass&) = default;

		Application* m_Application = nullptr;
		Technique* m_Technique = nullptr;
		ID3DX11EffectPass* m_Pass = nullptr;
		D3DX11_PASS_DESC m_PassDesc{};
		std::string m_Name{};
	};
}