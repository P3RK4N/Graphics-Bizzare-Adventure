#include "pch.h"
#include "CubeDemo.h"

#define VERTEX_COUNT 4
#define INDEX_COUNT 6 * 2 * 3

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(CubeDemo)

	void CubeDemo::initialize()
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

		//Creating buffers
		BasicEffectVertex vertices[] =
		{
			{ XMFLOAT4(-1.0f,1.0f, -1.0f, 1.0f), XMFLOAT4(0.2f, 0.4f, 0.8f, 1.0f) },
			{ XMFLOAT4(1.0f, 1.0f, -1.0f, 1.0f), XMFLOAT4(0.8f, 0.2f, 0.3f, 1.0f) },
			{ XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.1f, 0.7f, 0.2f, 1.0f) },
			{ XMFLOAT4(-1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT4(0.5f, 0.7f, 0.9f, 1.0f) },
			{ XMFLOAT4(-1.0f,-1.0f, 1.0f, 1.0f), XMFLOAT4(0.2f, 0.3f, 0.6f, 1.0f) },
			{ XMFLOAT4(1.0f, -1.0f, 1.0f, 1.0f), XMFLOAT4(0.8f, 0.5f, 0.2f, 1.0f) },
			{ XMFLOAT4(1.0f,-1.0f, -1.0f, 1.0f), XMFLOAT4(0.5f, 0.7f, 0.2f, 1.0f) },
			{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1.0f), XMFLOAT4(0.1f, 0.2f, 0.6f, 1.0f) },
		};

		UINT16 indices[INDEX_COUNT] =
		{
			0, 1, 2,
			0, 2, 3,
			4, 5, 6,
			4, 6, 7,
			3, 2, 5,
			3, 5, 4,
			2, 1, 6,
			2, 6, 5,
			1, 7, 6,
			1, 0, 7,
			0, 3, 4,
			0, 4, 7
		};


		D3D11_BUFFER_DESC vertexBufferDesc =
		{
			sizeof(vertices),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		D3D11_BUFFER_DESC indexBufferDesc =
		{
			sizeof(indices),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData =
		{
			vertices,
			0,
			0
		};

		D3D11_SUBRESOURCE_DATA indexBufferData =
		{
			indices,
			0,
			0
		};

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateBuffer
		(
			&vertexBufferDesc,
			&vertexBufferData,
			&m_VertexBuffer
		))) throw new ApplicationException("CreateBuffer() error", hr);

		if(FAILED(hr = m_Application->getDirect3DDevice()->CreateBuffer
		(
			&indexBufferDesc,
			&indexBufferData,
			&m_IndexBuffer
		))) throw new ApplicationException("CreateBuffer() error", hr);

		DrawableApplicationComponent::initialize();
	}

	void CubeDemo::draw(const ApplicationTime& applicationTime)
	{
		//Set primitive topology for input assembler stage
		ID3D11DeviceContext1* deviceContext = m_Application->getDirect3DDeviceContext();
		deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//Bind input layout to input assembler stage
		deviceContext->IASetInputLayout(m_InputLayout);
		//Bind vertex buffer to input assembler stage
		UINT stride = sizeof(BasicEffectVertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);
		//Bind index buffer to input assembler stage
		deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		//Set shader constants
		XMMATRIX worldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		XMMATRIX wvp = worldMatrix * m_Camera->getViewProjectionMatrix();
		//TMP
		worldMatrix = XMMatrixRotationY(1.0f * (float)applicationTime.getElapsedApplicationTime()) * worldMatrix;
		XMStoreFloat4x4(&m_WorldMatrix, worldMatrix);
		//----
		if(FAILED(hr = m_WVPVariable->SetMatrix((const float*)&wvp)))
			throw new ApplicationException("SetRawValue() error", hr);
		//Apply effect pass
		m_Pass->Apply(0, deviceContext);
		//Execute indexed draw
		deviceContext->DrawIndexed(INDEX_COUNT, 0, 0);
		//deviceContext->Draw(VERTEX_COUNT, 0);
	}

	CubeDemo::~CubeDemo()
	{
		ReleaseObject(m_WVPVariable);
		ReleaseObject(m_Pass);
		ReleaseObject(m_Technique);
		ReleaseObject(m_Effect);
		ReleaseObject(m_InputLayout);
		ReleaseObject(m_VertexBuffer);
		ReleaseObject(m_IndexBuffer);
	}
}
