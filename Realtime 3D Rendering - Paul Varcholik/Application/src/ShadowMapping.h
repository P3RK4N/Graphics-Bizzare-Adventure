#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{
	class ShadowMapping : public Application
	{
	public:
		ShadowMapping(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~ShadowMapping() {}

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void shutdown() override;

		bool isShadowPass() const override { return m_IsShadowPass; }
		RenderTarget* getRenderTarget() const { return m_ShadowMappingTarget; }

	protected:
		LPDIRECTINPUT8 m_DirectInput = nullptr;

	protected:
		TextPrinter* m_TextPrinter = nullptr;
		FPSComponent* m_FPSComponent = nullptr;

		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;

		FirstPersonCamera* m_FirstPersonCamera = nullptr;
		Camera* m_LightCamera = nullptr;

		Skybox* m_Skybox = nullptr;

		DiffuseLightingDemo* m_DiffuseLightingDemo = nullptr;
		PointLightDemo* m_PointLightDemo = nullptr;
		ProjectionDemo* m_ProjectionDemo1 = nullptr;
		ProjectionDemo* m_ProjectionDemo2 = nullptr;

		DepthMap* m_ShadowMappingTarget = nullptr;
		bool m_IsShadowPass = false;

	private:
		static const DirectX::XMVECTORF32 s_BackgroundColor;

		void updateText();
	};
}