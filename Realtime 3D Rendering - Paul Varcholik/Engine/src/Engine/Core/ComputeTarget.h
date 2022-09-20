#pragma once
#include "Application.h"

#include "Engine/Materials/ComputeMaterial.h"

namespace Engine
{
	class ComputeTarget
	{
	public:
		ComputeTarget(Application* app);
		~ComputeTarget();

		ID3D11ShaderResourceView* getOutputColor() const { return m_OutputColor; }
		ID3D11UnorderedAccessView* getUAV() const { return m_UAV; }

		void draw(const ApplicationTime& applicationTime);

	private:
		ComputeTarget() = default;
		ComputeTarget(const ComputeTarget&) = default;
		ComputeTarget& operator=(const ComputeTarget&) = default;

		Application* m_Application = nullptr;
		ID3D11UnorderedAccessView* m_UAV = nullptr;
		ID3D11ShaderResourceView* m_OutputColor = nullptr;

		ComputeMaterial* m_ComputeMaterial = nullptr;
		Effect* m_ComputeEffect = nullptr;
		Pass* m_ComputePass = nullptr;

		XMFLOAT2 m_TextureSize = {};
		XMUINT2 m_ThreadGroupCount{ 32, 32 };
	};
}