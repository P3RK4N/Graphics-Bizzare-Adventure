#include "pch.h"
#include "Pass.h"

#include "Engine/Core/Application.h"

namespace Engine
{
	static HRESULT hr;

	Pass::Pass(Application* app, Technique* technique, ID3DX11EffectPass* pass)
		: m_Application(app), m_Technique(technique), m_Pass(pass)
	{
		m_Pass->GetDesc(&m_PassDesc);
		m_Name = m_PassDesc.Name;
	}

	Pass::~Pass()
	{
		ReleaseObject(m_Pass);
	}

	void Pass::createInputLayout(const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, UINT numElements, ID3D11InputLayout** inputLayout)
	{
		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateInputLayout
		(
			inputElementDesc,
			numElements,
			m_PassDesc.pIAInputSignature,
			m_PassDesc.IAInputSignatureSize,
			inputLayout
		))) throw new ApplicationException();
	}

	void Pass::apply(UINT flags, ID3D11DeviceContext* context)
	{
		m_Pass->Apply(flags, context);
	}
}
