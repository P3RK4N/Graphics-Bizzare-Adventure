#pragma once
#include <Engine.h>
#include <vector>

#include <dinput.h>

namespace Engine
{
	class Geometry_Tesselation_Game : public Application
	{
	public:
		Geometry_Tesselation_Game(HINSTANCE instance, const std::wstring& windowClass, const std::wstring& windowTitle, int showCommand)
			: Application(instance, windowClass, windowTitle, showCommand)
		{
			m_IsDepthStencilBufferEnabled = true;
			m_IsMultiSamplingEnabled = true;
		}

		virtual ~Geometry_Tesselation_Game() {}

		virtual void initialize() override;
		virtual void update(const ApplicationTime& applicationTime) override;
		virtual void draw(const ApplicationTime& applicationTime) override;
		virtual void shutdown() override;

	protected:
		LPDIRECTINPUT8 m_DirectInput = nullptr;

	protected:
		Keyboard* m_Keyboard = nullptr;
		Mouse* m_Mouse = nullptr;

		FPSComponent* m_FPSComponent = nullptr;
		TextPrinter* m_TextPrinter = nullptr;

		FirstPersonCamera* m_FirstPersonCamera = nullptr;
		Skybox* m_Skybox = nullptr;

		GeometryMaterialDemo* m_GeometryMaterialDemo1 = nullptr;
		GeometryMaterialDemo* m_GeometryMaterialDemo2 = nullptr;
		GeometryMaterialDemo* m_GeometryMaterialDemo3 = nullptr;

	private:
		static const DirectX::XMVECTORF32 s_BackgroundColor;
	};
}