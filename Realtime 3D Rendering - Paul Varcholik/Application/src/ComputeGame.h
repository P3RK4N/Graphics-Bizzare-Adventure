#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{

	class ComputeGame : public Application
	{
	public:
		ComputeGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~ComputeGame() {}

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void shutdown() override;

	protected:
		LPDIRECTINPUT8 m_DirectInput = nullptr;

		FPSComponent* m_FPSComponent = nullptr;
		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;
		FirstPersonCamera* m_FirstPersonCamera = nullptr;
		Skybox* m_Skybox = nullptr;
		DiffuseLightingDemo* m_DiffuseLightingDemo = nullptr;
		PointLightDemo* m_PointLightDemo = nullptr;

		ComputeTarget* m_ComputeTarget = nullptr;			
		FullScreenQuad* m_FullScreenQuad = nullptr;
		Effect* m_PostProcessEffect = nullptr;
		PostProcessMaterial* m_PostProcessMaterial = nullptr;

		ID3D11Resource* m_DistortionTexture = nullptr;
		ID3D11ShaderResourceView* m_DistortionView = nullptr;


		void updatePostProcessMaterial();

	private:

		static const DirectX::XMVECTORF32 s_BackgroundColor;

	};
}