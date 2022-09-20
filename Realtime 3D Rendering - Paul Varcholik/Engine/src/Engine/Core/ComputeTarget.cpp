#include "pch.h"
#include "ComputeTarget.h"

namespace Engine
{
	static HRESULT hr1;

	ComputeTarget::ComputeTarget(Application* app)
		: m_Application(app)
	{
		m_ComputeEffect = new Effect(app);
		m_ComputeEffect->loadCompiledEffect(L"resources\\ComputeShader.cso");
		m_ComputeMaterial = new ComputeMaterial();
		m_ComputeMaterial->initialize(m_ComputeEffect);
		m_ComputePass = m_ComputeMaterial->getCurrentTechnique()->getPasses().at(0);

		m_TextureSize = { (float)m_Application->getWidth(), (float)m_Application->getHeight() };
		m_TextureSize = { 2560.0f, 1440.0f };

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(textureDesc));

		textureDesc.Width = app->getWidth();
		textureDesc.Height = app->getHeight();
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

		ID3D11Texture2D* texture = nullptr;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateTexture2D(&textureDesc, nullptr, &texture)))
			throw new ApplicationException();

		D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
		ZeroMemory(&uavDesc, sizeof(uavDesc));

		uavDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateUnorderedAccessView(texture, &uavDesc, &m_UAV)))
			throw new ApplicationException();

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
		ZeroMemory(&resourceViewDesc, sizeof(resourceViewDesc));

		resourceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = 1;

		if(FAILED(hr1 = m_Application->getDirect3DDevice()->CreateShaderResourceView(texture, &resourceViewDesc, &m_OutputColor)))
			throw new ApplicationException();

		ReleaseObject(texture);
	}

	ComputeTarget::~ComputeTarget()
	{
		ReleaseObject(m_OutputColor);
	}

	void ComputeTarget::draw(const ApplicationTime& applicationTime)
	{
		ID3D11DeviceContext* dc = m_Application->getDirect3DDeviceContext();
		
		m_ComputeMaterial->getBlueColor() << 0.5f + 0.5 * sin(applicationTime.getTotalApplicationTime() * 20.0f);
		m_ComputeMaterial->getTextureSize() << XMLoadFloat2(&m_TextureSize);
		m_ComputeMaterial->getOutputTexture() << m_UAV;
		m_ComputePass->apply(0, dc);

		dc->Dispatch(m_ThreadGroupCount.x, m_ThreadGroupCount.y, 1);

		static ID3D11UnorderedAccessView* emptyUAV = nullptr;
		dc->CSSetUnorderedAccessViews(0, 1, &emptyUAV, nullptr);
	}
}
