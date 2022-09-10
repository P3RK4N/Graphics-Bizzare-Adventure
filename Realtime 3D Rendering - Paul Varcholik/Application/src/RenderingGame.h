#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{
	class RenderingGame : public Application
	{
	public:
		RenderingGame(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~RenderingGame() {}

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
		TriangleDemo* m_TriangleDemo = nullptr;
		CubeDemo* m_CubeDemo = nullptr;
		ModelDemo* m_ModelDemo = nullptr;
		TexturedModelDemo* m_TexturedModelDemo = nullptr;
		MaterialDemo* m_MaterialDemo = nullptr;
		Skybox* m_Skybox = nullptr;

	private:
		static const DirectX::XMVECTORF32 s_BackgroundColor;
	};
}