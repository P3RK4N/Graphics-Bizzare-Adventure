#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{
	class AnimationGame : public Application
	{
	public:
		AnimationGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~AnimationGame() {}

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void shutdown() override;

	protected:
		LPDIRECTINPUT8 m_DirectInput = nullptr;

	protected:
		TextPrinter* m_TextPrinter = nullptr;
		FPSComponent* m_FPSComponent = nullptr;
		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;
		FirstPersonCamera* m_FirstPersonCamera = nullptr;
		Skybox* m_Skybox = nullptr;
		PointLightDemo* m_PointLightDemo = nullptr;
		AnimationDemo* m_AnimationDemo = nullptr;

	private:
		static const DirectX::XMVECTORF32 s_BackgroundColor;
	};
}