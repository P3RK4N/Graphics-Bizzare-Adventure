#include "pch.h"
#include "TriangleDemo.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(TriangleDemo)

	void TriangleDemo::initialize()
	{
		//Compiling Shader
		UINT shaderFlags = 0;

#ifdef DEBUG
		shaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		ID3D10Blob* compiledShader = nullptr;
		ID3D10Blob* errorMessage = nullptr;
		
		if(FAILED(hr = D3DCompileFromFile
		(
			L"resources\\basic.fx",
			nullptr,
			nullptr,
			nullptr,
			"fx_5_0",
			shaderFlags,
			0,
			&compiledShader,
			&errorMessage
		))) throw new ApplicationException("D3DCompileFromFile() error", hr);

		//if(errorMessage != nullptr) 
		//{
		//	auto msg = (char*)errorMessage->GetBufferPointer();
		//	ReleaseObject(errorMessage);
		//	throw new ApplicationException(msg);
		//}

		if(FAILED(hr = D3DX11CreateEffectFromMemory
		(
			compiledShader->GetBufferPointer(),
			compiledShader->GetBufferSize(),
			0,
			m_Application->getDirect3DDevice(),
			&m_Effect
		))) throw new ApplicationException("D3DX11CreateEffectFromMemory() error", hr);

		ReleaseObject(compiledShader);

		//Getting technique, pass and cbuffer variables
		m_Technique = m_Effect->GetTechniqueByName("main11");
		if(!m_Technique) throw new ApplicationException("Could not get Technique");

		m_Pass = m_Technique->GetPassByName("p0");
		if(!m_Pass) throw new ApplicationException("Could not get Pass");

		m_WVPVariable = m_Effect->GetVariableByName("WorldViewProjection")->AsMatrix();
		if(!m_WVPVariable) throw new ApplicationException("Could not get WorldViewProjection Matrix");
		if(!m_WVPVariable->IsValid()) throw new ApplicationException("Could not cast to Matrix");

		//Creating Input Layout
		D3D11_INPUT_ELEMENT_DESC desc[2] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		
		D3DX11_PASS_DESC passDesc;
		if(FAILED(hr = m_Pass->GetDesc(&passDesc))) throw new ApplicationException("GetDesc() error", hr);

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateInputLayout
		(
			desc,
			sizeof(desc) / sizeof(desc[0]),
			passDesc.pIAInputSignature,
			passDesc.IAInputSignatureSize,
			&m_InputLayout
		))) throw new ApplicationException("CreateInputLayout() error", hr);

		//Creating buffer
		BasicEffectVertex vertices[3] =
		{
			{ XMFLOAT4(-1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.2f, 0.4f, 0.8f, 1.0f) },
			{ XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT4(0.8f, 0.2f, 0.3f, 1.0f) },
			{ XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT4(0.1f, 0.7f, 0.2f, 1.0f) }
		};

		D3D11_BUFFER_DESC bufferDesc =
		{
			sizeof(vertices),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		D3D11_SUBRESOURCE_DATA bufferData =
		{
			vertices,
			0,
			0
		};

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateBuffer
		(
			&bufferDesc,
			&bufferData,
			&m_VertexBuffer
		))) throw new ApplicationException("CreateBuffer() error", hr);

		DrawableApplicationComponent::initialize();
	}

	void TriangleDemo::draw(const ApplicationTime& applicationTime)
	{
		ID3D11DeviceContext1* deviceContext = m_Application->getDirect3DDeviceContext();

		//Set primitive topology for input assembler stage
		deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//Bind input layout to input assembler stage
		deviceContext->IASetInputLayout(m_InputLayout);
		//Bind vertex buffer to input assembler stage
		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		//Set shader constants
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();

		if(FAILED(hr = m_WVPVariable->SetMatrix((const float*)&wvp)))
			throw new ApplicationException("SetRawValue() error", hr);
		//Apply effect pass
		m_Pass->Apply(0, deviceContext);
		//Execute nonindexed draw
		deviceContext->Draw(3, 0);
	}

	TriangleDemo::~TriangleDemo()
	{
		ReleaseObject(m_WVPVariable);
		ReleaseObject(m_Pass);
		ReleaseObject(m_Technique);
		ReleaseObject(m_Effect);
		ReleaseObject(m_InputLayout);
		ReleaseObject(m_VertexBuffer);
	}
}
