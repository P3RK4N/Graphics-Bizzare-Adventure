#include "pch.h"
#include "ModelDemo.h"

#include "Engine/Core/Mesh.h"

namespace Engine
{
	static HRESULT hr;

	RTTI_DEFINITIONS(ModelDemo)

	void ModelDemo::initialize()
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

		Scope<Model> model = createScope<Model>(m_Application, "resources\\sphere.obj", true);

		Mesh* mesh = model->getMeshes().at(0);

		createVertexBuffer(m_Application->getDirect3DDevice(), *mesh, &m_VertexBuffer);
		mesh->createIndexBuffer(&m_IndexBuffer);
		m_IndexCount = mesh->getIndices().size();

		DrawableApplicationComponent::initialize();
	}

	void ModelDemo::draw(const ApplicationTime& applicationTime)
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
		deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
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
		deviceContext->DrawIndexed(m_IndexCount, 0, 0);
		//deviceContext->Draw(VERTEX_COUNT, 0);
	}

	void ModelDemo::createVertexBuffer(ID3D11Device* device, const Mesh& mesh, ID3D11Buffer** vertexBuffer) const
	{
		const std::vector<XMFLOAT3>& sourceVertices = mesh.getVertices();
		std::vector<BasicEffectVertex> vertices;
		vertices.reserve(sourceVertices.size());

		if(mesh.getVertexColors().size() > 0)
		{
			std::vector<XMFLOAT4>* vertexColors = mesh.getVertexColors().at(0);

			assert(vertexColors->size() == sourceVertices.size());

			for(UINT i = 0; i < sourceVertices.size(); i++)
			{
				XMFLOAT3 position = sourceVertices.at(i);
				XMFLOAT4 color = vertexColors->at(i);
				vertices.push_back(BasicEffectVertex{ XMFLOAT4{position.x, position.y, position.z, 1.0f} , color });
			}
		}
		else
		{
			for(UINT i = 0; i < sourceVertices.size(); i++)
			{
				XMFLOAT3 position = sourceVertices.at(i);
				vertices.push_back(BasicEffectVertex
					{ 
						XMFLOAT4{position.x, position.y, position.z, 1.0f}, 
						{ (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, 1.0f }
					});
			}
		}

		D3D11_BUFFER_DESC vertexBufferDesc =
		{
			sizeof(BasicEffectVertex) * vertices.size(),
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		D3D11_SUBRESOURCE_DATA vertexData =
		{
			&vertices[0],
			0,
			0
		};

		if(FAILED(hr = device->CreateBuffer(&vertexBufferDesc, &vertexData, vertexBuffer)))
			throw new ApplicationException("CreateBuffer() error", hr);
	}

	ModelDemo::~ModelDemo()
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
