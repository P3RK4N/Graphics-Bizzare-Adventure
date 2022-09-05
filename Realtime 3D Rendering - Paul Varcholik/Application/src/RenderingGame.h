#pragma once
#include <Engine.h>
#include <vector>

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

	private:
		static const DirectX::XMVECTORF32 s_BackgroundColor;
	};
}