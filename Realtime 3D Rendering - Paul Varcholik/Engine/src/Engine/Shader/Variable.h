#pragma once

#include <string>

#include <d3dx11effect.h>
#include <DirectXMath.h>

using namespace DirectX;

namespace Engine
{
	class Effect;

	class Variable
	{
	public:
		Variable(Effect* effect, ID3DX11EffectVariable* variable);
		~Variable();

		Effect* getEffect() { return m_Effect; }
		ID3DX11EffectVariable* getVariable() const { return m_Variable; }
		const D3DX11_EFFECT_VARIABLE_DESC& getVariableDesc() const { return m_VariableDesc; }
		ID3DX11EffectType* getType() const { return m_Type; }
		const D3DX11_EFFECT_TYPE_DESC& getTypeDesc() const { return m_TypeDesc; }
		const std::string& getName() const { return m_Name; }

		Variable& operator<<(CXMMATRIX value);
		Variable& operator<<(ID3D11ShaderResourceView* value);
		Variable& operator<<(FXMVECTOR value);
		Variable& operator<<(float value);

	private:
		Variable(const Variable&) = default;
		Variable& operator=(const Variable&) = default;

		Effect* m_Effect = nullptr;
		ID3DX11EffectVariable* m_Variable = nullptr;
		D3DX11_EFFECT_VARIABLE_DESC m_VariableDesc{};
		ID3DX11EffectType* m_Type = nullptr;
		D3DX11_EFFECT_TYPE_DESC m_TypeDesc{};
		std::string m_Name{};
	};
}