#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{

	class PostProcessing : public Application
	{
	public:
		PostProcessing(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~PostProcessing() {}

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void shutdown() override;

	protected:
		LPDIRECTINPUT8 m_DirectInput = nullptr;

		TextPrinter* m_TextPrinter = nullptr;
		FPSComponent* m_FPSComponent = nullptr;
		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;
		FirstPersonCamera* m_FirstPersonCamera = nullptr;
		Skybox* m_Skybox = nullptr;
		DiffuseLightingDemo* m_DiffuseLightingDemo = nullptr;
		PointLightDemo* m_PointLightDemo = nullptr;

		FullScreenRenderTarget* m_DepthMap = nullptr;			
		FullScreenQuad* m_FullScreenQuad = nullptr;
		Effect* m_PostProcessingEffect = nullptr;
		PostProcessMaterial* m_PostProcessMaterial = nullptr;

		ID3D11Resource* m_DistortionTexture = nullptr;
		ID3D11ShaderResourceView* m_DistortionView = nullptr;

		void updatePostProcessMaterial();

	private:

		static const DirectX::XMVECTORF32 s_BackgroundColor;

	};
}